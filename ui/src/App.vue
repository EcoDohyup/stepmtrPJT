<template>
  <div id="app">
    <h1>스테퍼 모터 원격 제어</h1>
    <div class="control-panel">
      <!-- Motor Control -->
      <div class="motor-control">
        <h2>모터 제어</h2>
        <p>상태: {{ status }}</p>
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
    async stopMotor() {
      try {
        const response = await fetch('http://192.168.0.98/motor/stop', {
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
        const response = await fetch(`http://192.168.0.98/motor/${direction}`, {
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
