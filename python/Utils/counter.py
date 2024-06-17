import multiprocessing
import time

class Counter:
    """
    thread safe counter implementation
    
    :param count: The count of the counter
    :param lock: The lock for the counter
    """
    def __init__(self) -> None:
        """
        initialize the counter
        """
        self.conut = multiprocessing.Value('i', 0)
        self.lock = multiprocessing.Lock()

    def increment(self) -> None:
        """
        increment the counter
        """
        with self.lock:
            self.conut.value += 1
    
    def decrement(self) -> None:
        """
        decrement the counter
        """
        with self.lock:
            self.conut.value -= 1
    
    def get_value(self) -> int:
        """
        get the current value of the counter
        """
        with self.lock:
            return self.conut.value