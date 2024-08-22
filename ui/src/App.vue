<template>
  <div id="app">
    <h1>Stepper Motor Control</h1>
    <div class="control-panel">
      <!-- Motor Control -->
      <div class="motor-control">
        <h2>Motor Control</h2>
        <p>Status: {{ status }}</p>
        <p>Speed: {{ speed }} (Use arrow buttons to adjust)</p>
        <button @click="changeSpeed(-1)">&#9664; Decrease Speed</button>
        <button @click="changeSpeed(1)">&#9654; Increase Speed</button>
        <button @click="stopMotor">Stop Motor</button>
        <button @click="moveMotor('right')">Move Right</button>
        <button @click="moveMotor('left')">Move Left</button>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'App',
  data() {
    return {
      status: 'Motor stopped',
      speed: 0 // Initial speed
    }
  },
  methods: {
    async changeSpeed(amount) {
      // Update the speed and send it to the backend
      this.speed = Math.max(0, this.speed + amount); // Ensure speed is not negative
      await this.sendSpeed();
    },
    async sendSpeed() {
      try {
        const response = await fetch(`http://192.168.0.94/motor/speed?value=${this.speed}`, {
          method: 'GET',
          mode: 'cors',
          headers: {
            'Content-Type': 'text/plain'
          }
        });
        if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
        }
        this.status = `Speed set to ${this.speed}`;
        console.log(this.speed);
      } catch (error) {
        console.error('Error setting speed:', error);
        this.status = `Error setting speed: ${error.message}`;
      }
    },
    async stopMotor() {
      try {
        const response = await fetch('http://192.168.0.94/motor/stop', {
          method: 'GET',
          mode: 'cors',
          headers: {
            'Content-Type': 'text/plain'
          }
        });
        if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
        }
        this.status = 'Motor stopped';
        this.speed = 0; // Reset speed to 0 when stopping
      } catch (error) {
        console.error('Error stopping motor:', error);
        this.status = `Error stopping motor: ${error.message}`;
      }
    },
    async moveMotor(direction) {
      try {
        const response = await fetch(`http://192.168.0.94/motor/${direction}`, {
          method: 'GET',
          mode: 'cors',
          headers: {
            'Content-Type': 'text/plain'
          }
        });
        if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
        }
        this.status = `Motor moving ${direction}`;
      } catch (error) {
        console.error('Error moving motor:', error);
        this.status = `Error moving motor: ${error.message}`;
      }
    }
  }
}
</script>

<style scoped>
#app {
  text-align: center;
  margin-top: 60px;
}

button {
  margin: 10px;
  padding: 10px;
}

p {
  margin-top: 20px;
}

h2 {
  margin-top: 20px;
}
</style>
