FROM python:3.13-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    && rm -rf /var/lib/apt/lists/*

COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt

COPY main.py .
COPY src/ src/
COPY resources/ resources/

RUN useradd -m appuser && chown -R appuser:appuser /app
USER appuser

CMD ["python", "main.py"]