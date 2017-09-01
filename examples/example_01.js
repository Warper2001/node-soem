var soem = require('../build/Release/node-soem');
    master = soem.NodeSoemMaster("enp0s8");
        
console.log('Initiating.', master.getInterfaceName());

console.log(master.init(), 'on interface', master.getInterfaceName());

console.log(master.configInit(), 'working counter on config_init call.');
console.log(master.configMap(), 'config map completed.');
console.log(master.configDC(), 'configuring dc.');


slaves = master.getSlaves()

console.log(slaves);

console.log(slaves[0].ec_sii[0])