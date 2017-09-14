var soem = require('../build/Release/node-soem');
master = soem.NodeSoemMaster("enp0s8");
    
console.log('Initiating.', master.getInterfaceName());

console.log(master.init(), 'on interface', master.getInterfaceName());

console.log(master.configInit(), 'working counter on config_init call.');
console.log(master.configMap(), 'config map completed.');
console.log(master.configDC(), 'configuring dc.');

slaves = master.getSlaves();

mySlaves = []


master.getSlaves().map((slave, index) => (
    mySlaves[index] = slave,
    mySlaves[index].Inputs = slave.ec_sii[0].variables,
    mySlaves[index].Outputs = slave.ec_sii[1].variables
))

console.log(mySlaves)

console.log(master.getAdapters())