FROM python:3.9-alpine

COPY requirements.txt .

RUN python3 -m venv venv
RUN source venv/bin/activate

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

EXPOSE 5000

CMD ["python3", "server.py"]