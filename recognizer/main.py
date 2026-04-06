import asyncio
import logging
import sys
from src.core.config import settings
from src.recognizers import VLLMRecognizer, StubRecognizer
from src.mqtt.handler import MQTTHandler

logging.basicConfig(
    level=settings.LOG_LEVEL,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)

RECOGNIZERS = {
    "vllm": VLLMRecognizer,
    "stub": StubRecognizer,
}

def main():
    if sys.platform == 'win32':
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

    recognizer_type = settings.RECOGNIZER_TYPE.lower()
    recognizer_class = RECOGNIZERS.get(recognizer_type)
    
    if not recognizer_class:
        logger.error(f"Unknown recognizer type: {recognizer_type}. Available: {list(RECOGNIZERS.keys())}")
        sys.exit(1)
    
    recognizer = recognizer_class()
    logger.info(f"Using recognizer: {recognizer_type}")
    
    mqtt_handler = MQTTHandler(recognizer)

    logger.info("Starting Recognizer Service...")
    try:
        asyncio.run(mqtt_handler.run())
    except KeyboardInterrupt:
        logger.info("Service interrupted by user.")
    except Exception as e:
        logger.error(f"Service encountered an error: {e}")
    finally:
        logger.info("Service stopped.")

if __name__ == "__main__":
    main()