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

master.writeState(1, 8);

console.log("State: ", master.statecheck(1, 8, 50))

address = 0x1520

setInterval(() => {
    // console.log(master.writeData(address, 8, [5]));
    // console.log(master.readData(address, 8));
    console.log(master.getSlaves()[0].outputs[0])
    // console.log(address);
    // address++
}, 500)
// intial send
master.sendProcessdata();
master.receiveProcessdata();


// console.log(master.writeData(5632, 8, [3256456]));
// console.log(master.readData(5632, 8));

// setInterval(() => {
//     console.log(master.readData(6656, 32));
// },1000)



// slaves = master.getSlaves()

// // console.log(slaves);

// console.log(slaves[0].ec_sii[1])