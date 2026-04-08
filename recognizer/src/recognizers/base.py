from abc import ABC, abstractmethod
from dataclasses import dataclass

@dataclass
class RecognitionResult:
    """Result of character recognition."""
    tag: str
    rec_alt: str

class BaseRecognizer(ABC):
    @abstractmethod
    async def recognize(self, image_base64: str) -> RecognitionResult:
        """
        Recognize character pairs (Letter+Digit) in the provided base64 encoded image.
        
        Args:
            image_base64: Base64 encoded image string.
            
        Returns:
            RecognitionResult with tag and rec_alt fields.
        """
        pass