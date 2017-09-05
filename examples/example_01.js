var soem = require('../build/Release/node-soem');
    master = soem.NodeSoemMaster("enp0s8");
        
console.log('Initiating.', master.getInterfaceName());

console.log(master.init(), 'on interface', master.getInterfaceName());

console.log(master.configInit(), 'working counter on config_init call.');
console.log(master.configMap(), 'config map completed.');
console.log(master.configDC(), 'configuring dc.');

slaves = master.getSlaves();

// intial send
master.sendProcessdata();
master.receiveProcessdata();

console.log('Going to Operational State.');

master.writeState(0, 8);

console.log("State: ", master.statecheck(0, 4, 50))

// intial send
master.sendProcessdata();
master.receiveProcessdata();


console.log(master.writeData(0x1600, 8, [32]));
console.log(master.readData(0x87645321, 8));

// setInterval(() => {
//     console.log(master.readData(6656, 32));
// },1000)



// slaves = master.getSlaves()

// // console.log(slaves);

// console.log(slaves[0].ec_sii[1])