<template>
  <div id="app">
    <h1>스테퍼 모터 원격 제어</h1>
    <div class="control-panel">
      <!-- Motor Control -->
      <div class="motor-control">
        <h2>모터 제어</h2>
        <p>상태: {{ status }}</p>
        <p>속도: {{ speed }} (클릭하여 속도 조절)</p>
        <button @click="changeSpeed(-1)">&#9650; 감속</button>
        <button @click="changeSpeed(1)">&#9660; 가속</button>
        <button @click="stopMotor">&#9208; 모터 정지</button>
        <button @click="moveMotor('left')">&#9664; 좌로 이동</button>
        <button @click="moveMotor('right')">&#9654; 우로 이동</button>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'App',
  data() {
    return {
      status: '모터 정지',
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
        this.status = `속도 세팅 : ${this.speed}`;
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
        this.status = '모터 정지';
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
        this.status = `${direction}로 이동중`;
      } catch (error) {
        console.error('Error moving motor:', error);
        this.status = `Error moving motor: ${error.message}`;
      }
    },
    handleKeydown(event) {
      switch (event.key) {
        case 'ArrowUp':
          this.changeSpeed(1);
          break;
        case 'ArrowDown':
          this.changeSpeed(-1);
          break;
        case ' ':
          this.stopMotor();
          break;
        case 'ArrowRight':
          this.moveMotor('right');
          break;
        case 'ArrowLeft':
          this.moveMotor('left');
          break;
        default:
          break;
      }
    }
  },
  mounted() {
    // Add keydown event listener when component is mounted
    window.addEventListener('keydown', this.handleKeydown);
  },
  beforeUnmount() {
    // Remove keydown event listener when component is destroyed
    window.removeEventListener('keydown', this.handleKeydown);
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
