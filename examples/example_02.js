var soem = require('../build/Release/node-soem');
master = soem.NodeSoemMaster("enp0s8");
    
console.log('Initiating.', master.getInterfaceName());

console.log(master.init(), 'on interface', master.getInterfaceName());

console.log(master.configInit(), 'working counter on config_init call.');
console.log(master.configMap(), 'config map completed.');
console.log(master.configDC(), 'configuring dc.');

console.log(master.getSlaves());


console.log('Sending initial data.');

// intial send
master.sendProcessdata();
master.receiveProcessdata();

console.log('Going to Operational State.');

master.writeState(0, 8);

var setupLoop = function () {

var setupInt = setInterval(function () {

    master.sendProcessdata();
    master.receiveProcessdata();
    var state = master.statecheck(1, 8, 50);

    if (state === 8) {
        clearInterval(setupInt);
        startLoop();
    }

}, 50);
};

var startLoop = function () {

var slaves = master.getSlaves(),
    counter = 0;

var int = setInterval(function () {

    master.sendProcessdata();
    var expectedWkc = master.getExpectedWC(), 
        wkc = master.receiveProcessdata();

    var dvo = new DataView(slaves[0].outputs),
        dvi = new DataView(slaves[0].inputs, 0, slaves[0].Ibytes);

    var str = '';

    address = slaves[0].ec_sii[0].variables[0].absOffset
    dataType = slaves[0].ec_sii[0].variables[0].dataType

    const readData = (variable) => {
        const { dataType, absOffset } = variable 
        console.log(dvi)
     
        switch(dataType) {
         case 'UNSIGNED32':
           return dvi.getUint32(absOffset, true)
         default:
             str = "NONE"
         }
     }

     const writeData = (variable, value) => {
        const { dataType, absOffset } = variable 
     
        switch(dataType) {
         case 'UNSIGNED8':
           dvo.setUint8(absOffset, value)
         default:
             str = "NONE"
         }
     }

    //  writeData(slaves[0].ec_sii[1].variables[0], 7)

    console.log(readData(slaves[0].ec_sii[0].variables[0]))

    
    if (expectedWkc !== wkc) {
    
        console.log('Working counter error.');

        clearInterval(startLoop);

        setupLoop();
        return;

    }

}, 5000);

};

setTimeout(() => {
    setupLoop();
},1000)
