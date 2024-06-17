from enum import Enum
from counter import Counter
from multiprocessing import Lock



class MetricType(Enum):
    READLOCKHOLDER = 1          # "Number of read lock holders"
    WRITELOCKHOLDER = 2         # "Number of write lock holders"
    HISTOGRAM = 3               # "Histogram"
    
    def __str__(self) -> str:
        return self.value

class MetricImpl:
    """
    base Metric class
    
    :param metric_name: The name of the metric
    :param metric_description: The description of the metric
    """
    def __init__(self, metric_name: str, metric_description: str) -> None:
        self.__counter = Counter()
    
    def get_value(self) -> int:
        """
        get the current value of the metric
        """
        return self.__counter.get_value()
    
    def increment(self) -> None:
        """
        increment the metric
        """
        self.__counter.increment()
        
    def decrement(self) -> None:
        """
        decrement the metric
        """
        self.__counter.decrement()
    
    def __enter__(self):
        self.increment()
    
    def __exit__(self, type, value, traceback):
        self.decrement()
    
class Metrics:
    """
    Implementation of the singleton metrics class
    """
    _instance = None
    _lock = Lock()
    map = {}

    def __new__(cls, *args, **kwargs) -> None:
        if not cls._instance:
            with cls._lock:
                if not cls._instance:
                    cls._instance = super(Metrics, cls).__new__(cls)
                    for metric_name, metric_value in MetricType.__members__.items():
                        cls.map[metric_value] = MetricImpl(metric_name, "")
                    return cls._instance
        return cls._instance
    
    def __init__(self) -> None:
        pass

    @classmethod
    def get_metric(cls, metric_type: MetricType) -> MetricImpl:
        """
        return the metric object for the given metric type
        """
        if not cls._instance:
            cls._instance = Metrics()
        with cls._lock:
            if metric_type not in cls.map:
                raise RuntimeError("Invalid metric type")
            return cls.map[metric_type]