import logging
import queue

from multiprocessing import Lock, Condition, Manager, Value
from enum import Enum
from metric import Metrics, MetricType
from uuid import uuid4

"""
Draft of the RwLock implementation
"""

class RwLockType(Enum):
    """
    RWlock type enumeration
    """
    READ = 1
    WRITE = 2

class LockHolder:
    """
    The LockHolder class is a simple implementation of a lock holder.
    
    :param owner_id: The id of the lock holder
    :param lock_type: The type of lock to hold
    """
    def __init__(self, owner_id: str, lock_type: RwLockType, rwlock):
        self.owner_id = owner_id
        self.metric = Metrics.get_metric(MetricType.READLOCKHOLDER) if lock_type == RwLockType.READ else Metrics.get_metric(MetricType.WRITELOCKHOLDER)
        self.parent = None
        self.bind = False
        self.iter_group = None

    def bindWith(self, rwlock, iter_group):
        self.metric.increment()
        self.bind = True
        self.parent = rwlock
        self.iter_group = iter_group
        self.iter_group.requests += 1
    
    def __del__(self):
        if self.bind and self.parent is not None:
            self.metric.decrement()
            self.parent.release(self)
    

class RwLockImpl:
    """
    The RwLockImpl class is a simple implementation of a read-write lock. reference Phase Fair Reader-Writer Locks
    (Phase Fair RWLock as suggested in https://www.cs.unc.edu/~anderson/papers/rtsj10-for-web.pdf)
    :FIXME: The RwLockImpl class is not process safe
    
    :param _instance: The instance of the RwLockImpl class
    :param _lock: The lock for the RwLockImpl class
    :internal_lock: The internal lock for the RwLockImpl class
    :param reader_queues: The list of reader queues
    :param writer_queues: The list of writer queues
    :param reader_owner: The reader owner
    :param writer_owner: The writer owner
    """
    
    _instance = None
    _lock = Lock()
    
    def __new__(cls) -> None:
        if cls._instance == None:
            print("Create RwLockImpl instance")
            with cls._lock:
                if cls._instance == None:
                    cls._instance = super(RwLockImpl, cls).__new__(cls)
                    return cls._instance
        return cls._instance
    
    class Group:
        def __init__(self, type: RwLockType) -> None:
            self.lock_type = type        # type: RwLockType
            self.requests = 0            # type: int
            self.cv = Condition()        # type: multiprocessing.Condition
        def __enter__(self):
            self.cv.acquire()
            self.requests += 1
        
        def __exit__(self, type, value, traceback):
            self.cv.release()
            self.requests -= 1
    
    def __init__(self):
        self.internal_lock = Lock()
        self.reader_queues = list()
        self.writer_queues = list()
        self.reader_owner = None
        self.writer_owner = None

    def release(self, lockholder: LockHolder):
        with self.internal_lock:
            if self.writer_owner == None and self.reader_owner == None:
                raise RuntimeError("Invalid lock holder")
            if self.writer_owner != None and self.reader_owner != None:
                raise RuntimeError("Invalid lock holder")
            if lockholder.iter_group != self.reader_owner and lockholder.iter_group != self.writer_owner:
                raise RuntimeError("Invalid lock holder")

            lockholder.iter_group.requests -= 1
            if lockholder.iter_group.requests == 0:
                self.releaseLockHolder(lockholder.iter_group)

    def acquireLock(self, lock_type: RwLockType) -> LockHolder:
        lock_holder = LockHolder(str(uuid4()), lock_type, self)
        need_wait = True
        with self.internal_lock:
            if lock_type == RwLockType.WRITE:
                self.writer_queues.append(RwLockImpl.Group(lock_type))
            elif len(self.reader_queues) == 0 or (self.reader_queues[0] == self.reader_owner and len(self.reader_queues) == 1 and not len(self.writer_queues) == 0):
                self.reader_queues.append(RwLockImpl.Group(lock_type))
            
            iter_group = self.reader_queues[-1] if lock_type == RwLockType.READ else self.writer_queues[-1]

            if self.reader_owner == None and self.writer_owner == None:
                need_wait = False
                if lock_type == RwLockType.WRITE:
                    self.writer_owner = iter_group
                else:
                    self.reader_owner = iter_group

        if need_wait:
            predicate = lambda x, y: x == y
            with iter_group:
                iter_group.cv.wait_for(lambda: predicate(self.reader_owner if lock_type == RwLockType.READ else self.writer_owner, iter_group))
        
        with self.internal_lock:
            lock_holder.bindWith(self, iter_group)
            return lock_holder
  
    def releaseLockHolder(self, iter_group: Group):
        self.writer_owner = None
        self.reader_owner = None
        if iter_group.lock_type == RwLockType.READ:
            self.reader_queues.remove(iter_group)
            if not len(self.writer_queues) == 0:
                self.writer_owner = self.writer_queues[0]
            elif not len(self.reader_queues) == 0:
                self.reader_owner = self.reader_queues[0]
        else:
            self.writer_queues.remove(iter_group)
            if not len(self.reader_queues) == 0:
                self.reader_owner = self.reader_queues[0]
            elif not len(self.writer_queues) == 0:
                self.writer_owner = self.writer_queues[0]
        if self.reader_owner != None:
            with self.reader_owner:
                self.reader_owner.cv.notify_all()
        elif self.writer_owner != None:
            with self.writer_owner:
                self.writer_owner.cv.notify_all()