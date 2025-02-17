from flask import Flask, request, jsonify, render_template
import serial
import threading
import json

app = Flask(__name__)
data = {"tempo_ms": 0, "posicao": 0, "deslocamento_mm": 0.0, "velocidade_mm_s": 0.0}

# Configurar a conexão serial
ser = serial.Serial('/dev/ttyUSB0', 115200)

def read_serial():
    global data
    while True:
        line = ser.readline().decode('utf-8').strip()
        try:
            data = json.loads(line)
        except json.JSONDecodeError:
            pass

@app.route('/')
def index():
    return render_template('index.html', data=data)

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(data)

if __name__ == '__main__':
    threading.Thread(target=read_serial).start()
    app.run(debug=True, host='0.0.0.0')