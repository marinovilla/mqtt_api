from flask import Flask, render_template, request
import paho.mqtt.client as mqtt

app = Flask(__name__)

mqtt_broker = "192.168.0.3"  # IP de la Raspberry Pi
mqtt_port = 1883
mqtt_topic_prefix = "ardu_pin"  # Topic prefix para los pines
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Conectado al broker MQTT")

client.on_connect = on_connect
client.connect(mqtt_broker, mqtt_port, 60)
client.loop_start()

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/control", methods=["POST"])
def control():
    pin_number = request.form.get("pin")
    action = request.form.get("action")
    mqtt_topic = f"{mqtt_topic_prefix}{pin_number}"
    client.publish(mqtt_topic, action)
    return ("", 204)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
