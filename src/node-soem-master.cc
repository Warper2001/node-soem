// node-soem-master.h

#include <node.h>
#include <nan.h>

#include <stdlib.h>
#include <string>
#include "ethercat.h"


using namespace v8;

class NodeSoemMaster : public Nan::ObjectWrap {

    public:
        static NAN_MODULE_INIT(Init) {
            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("NodeSoemMaster").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            SetPrototypeMethod(tpl, "init", init);
            SetPrototypeMethod(tpl, "configInit", configInit);
            SetPrototypeMethod(tpl, "configMap", configMap);
            SetPrototypeMethod(tpl, "configDC", configDC);

            SetPrototypeMethod(tpl, "sendProcessdata", sendProcessdata);
            SetPrototypeMethod(tpl, "receiveProcessdata", receiveProcessdata);

            SetPrototypeMethod(tpl, "writeState", writeState);
            SetPrototypeMethod(tpl, "readState", readState);
            SetPrototypeMethod(tpl, "statecheck", statecheck);

            SetPrototypeMethod(tpl, "getSlaves", getSlaves);
            SetPrototypeMethod(tpl, "getInterfaceName", getInterfaceName);
            SetPrototypeMethod(tpl, "getMap", getMap);

            SetPrototypeMethod(tpl, "getExpectedWC", getExpectedWC);

            SetPrototypeMethod(tpl, "writeData", writeData);
            SetPrototypeMethod(tpl, "readData", readData);
            SetPrototypeMethod(tpl, "getAdapters", getAdapters);

            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());

            Nan::Set(target, Nan::New("NodeSoemMaster").ToLocalChecked(),
                    Nan::GetFunction(tpl).ToLocalChecked());

        }

    private:
        explicit NodeSoemMaster(char *ifname) : ifname_(ifname) { }
        ~NodeSoemMaster() { }

        static NAN_METHOD(New) {

            if (info.IsConstructCall()) {
                // invoke as constructor: 'new NodeSoemMaster(...)'
                // get interface name from arguments

                char *ifname = (char*) calloc(1, 20);

                if (info[0]->IsUndefined() || !info[0]->IsString()) {

                    ifname = (char *) "eth0";

                } else {

                    String::Utf8Value str(info[0]->ToString());

                    strcpy(ifname, (char*) *str);

                }

                NodeSoemMaster* obj = new NodeSoemMaster(ifname);

                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());

            } else {

                // invoked as plain function 'NodeSoemMaster(...)', turn into constructor call.

                const int argc = 1;
                Local<Value> argv[argc] = { info[0] };
                Local<Function> cons = Nan::New(constructor());
                info.GetReturnValue().Set(cons->NewInstance(argc, argv));

            }

        }

        static NAN_METHOD(init) {
            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            int retVal = ec_init(obj->ifname_);

            info.GetReturnValue().Set(Number::New(isolate, retVal));
        }

        static NAN_METHOD(configInit) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_config_init(FALSE);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(configMap) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            int retVal = ec_config_map(&obj->ioMap_);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(configDC) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_configdc();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(sendProcessdata) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_send_processdata();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(receiveProcessdata) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_receive_processdata(5000);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(writeState) {

            Isolate* isolate = info.GetIsolate();

            int slave;
            int reqstate;


            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No slave defined in writeState.")));
                return;
            }

            slave = info[0]->Uint32Value();

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No state defined in writeState.")));
                return;
            }

            reqstate = info[1]->Uint32Value();

            ec_slave[slave].state = reqstate;

            int retVal = ec_writestate(slave);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(readState) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_readstate();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(statecheck) {

            Isolate* isolate = info.GetIsolate();

            int slave;
            int reqstate;
            int timeout = 1000;

            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No slave defined in statecheck.")));
                return;
            }

            slave = info[0]->Uint32Value();

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No requested state in statecheck.")));
            }

            reqstate = info[1]->Uint32Value();

            int retVal = ec_statecheck(slave, reqstate, timeout);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(getSlaves) {

            Isolate* isolate = info.GetIsolate();

            Local<Array> arr = Array::New(isolate);

            int i = 1;

            while (i <= ec_slavecount) {

                Local<Object> slave = Object::New(isolate);

                slave->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, ec_slave[i].name));
                slave->Set(String::NewFromUtf8(isolate, "state"), Number::New(isolate, ec_slave[i].state));
                slave->Set(String::NewFromUtf8(isolate, "ALStatusocde"), Number::New(isolate, ec_slave[i].ALstatuscode));
                slave->Set(String::NewFromUtf8(isolate, "configadr"), Number::New(isolate, ec_slave[i].configadr));
                slave->Set(String::NewFromUtf8(isolate, "aliasadr"), Number::New(isolate, ec_slave[i].aliasadr));

                int numbytes = ec_slave[i].Obytes;
                if ((numbytes == 0) && (ec_slave[i].Obits > 0)) numbytes = 1;
                if (numbytes > 8) numbytes = 8;

                slave->Set(String::NewFromUtf8(isolate, "Obits"), Uint32::New(isolate, ec_slave[i].Obits));
                slave->Set(String::NewFromUtf8(isolate, "Obytes"), Uint32::New(isolate, ec_slave[i].Obytes));
                slave->Set(String::NewFromUtf8(isolate, "outputs"),
                        ArrayBuffer::New(isolate, (void*) ec_slave[i].outputs, numbytes));

                numbytes = ec_slave[i].Ibytes;
                if ((numbytes == 0) && (ec_slave[i].Ibits > 0)) numbytes = 1;
                if (numbytes > 8) numbytes = 8;

                slave->Set(String::NewFromUtf8(isolate, "Ibits"), Uint32::New(isolate, ec_slave[i].Ibits));
                slave->Set(String::NewFromUtf8(isolate, "Ibytes"), Uint32::New(isolate, ec_slave[i].Ibytes));
                slave->Set(String::NewFromUtf8(isolate, "inputs"),
                        ArrayBuffer::New(isolate, (void*) ec_slave[i].inputs, numbytes));

                slave->Set(String::NewFromUtf8(isolate, "pdelay"), Uint32::New(isolate, ec_slave[i].pdelay));

                // slave->Set(String::NewFromUtf8(isolate, "party"), String::NewFromUtf8(isolate, ec_slave[i].ec_sii[0].ec_sii_variable[0].name));
                
                Local<Array> arr_ec_sii = Array::New(isolate);
                // Local<Object> ec_sii = Object::New(isolate);

                int ec_sii_counter = 0;
                while(ec_slave[i].ec_sii[ec_sii_counter].Address != 0) {

                    Local<Object> ec_sii = Object::New(isolate);
                    ec_sii->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, ec_slave[i].ec_sii[ec_sii_counter].Name));
                    ec_sii->Set(String::NewFromUtf8(isolate, "SM"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].SM));
                    ec_sii->Set(String::NewFromUtf8(isolate, "address"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].Address));

                    Local<Array> arr_ec_sii_variable = Array::New(isolate);
                    int ec_sii_variable_counter = 0;
                    while(ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].obj_idx != 0) {

                        Local<Object> ec_sii_variable = Object::New(isolate);
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].name));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "absOffset"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].abs_offset));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "absBit"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].abs_bit));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "objIndex"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].obj_idx));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "objSubIndex"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].obj_subidx));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "bitlen"), Uint32::New(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].bitlen));
                        ec_sii_variable->Set(String::NewFromUtf8(isolate, "dataType"), String::NewFromUtf8(isolate, ec_slave[i].ec_sii[ec_sii_counter].ec_sii_variable[ec_sii_variable_counter].dataType));

                        arr_ec_sii_variable->Set(ec_sii_variable_counter, ec_sii_variable);
                        ec_sii_variable_counter++;
                    }
                    
                    ec_sii->Set(String::NewFromUtf8(isolate, "variables"), arr_ec_sii_variable);

                    arr_ec_sii->Set(ec_sii_counter, ec_sii);
                    ec_sii_counter++;
                } 

                slave->Set(String::NewFromUtf8(isolate, "ec_sii"), arr_ec_sii);

                arr->Set(i-1, slave);

                i++;

            }

            info.GetReturnValue().Set(arr);

        }

        static NAN_METHOD(getInterfaceName) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            info.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->ifname_));

        }


        static NAN_METHOD(getMap) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, (void*)&obj->ioMap_, 4096);

            info.GetReturnValue().Set(buffer);

        }

        static NAN_METHOD(getExpectedWC) {

            Isolate* isolate = info.GetIsolate();

            int exp = ec_group[0].outputsWKC * 2 + ec_group[0].inputsWKC;

            info.GetReturnValue().Set(Uint32::New(isolate, exp));

        }

        static NAN_METHOD(writeData) {
            
            Isolate* isolate = info.GetIsolate();


            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No Address Defined")));
                return;
            }

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No Bitlength Defined")));
                return;
            }

            if (info[2]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No Data Defined")));
                return;
            }

            Local<Array> data = Local<Array>::Cast(info[2]);
            int address = info[0]->Uint32Value();
            int bitLength = info[1]->Uint32Value();
            int dataOut[1024];
            
            
            for(unsigned int i = 0; i < data->Length(); i++) {
                int val = data->Get(i)->Uint32Value();
                printf("BufferData %d\n",  val);
                dataOut[i] = val;
            }
 

            // int dataIn[1024];

            int success = ecx_BWR(&ecx_port,0, address, bitLength, dataOut, 1024);

            // success = ecx_BRD(&ecx_port,0, 5632, 8, dataIn, 1024);
            
            // // printf("Dataout: %d\n", dataOut->Get(0));
            // printf("Datain: %d\n", dataIn[0]);
            // printf("Datain: %d\n", dataIn[1]);

            info.GetReturnValue().Set(Uint32::New(isolate, success));

        }

        static NAN_METHOD(readData) {
            
            Isolate* isolate = info.GetIsolate();

            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No Address Defined")));
                return;
            }

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No Bitlength Defined")));
                return;
            }

            int address = info[0]->Uint32Value();
            int bitLength = info[1]->Uint32Value();

            int dataIn[1024];

            // int dataOut[1024];

            // dataOut[0] = 4;

            // int success = ecx_BWR(&ecx_port,0, 5632, 8, dataOut, 1024);

            int success = ecx_BRD(&ecx_port,0, address, bitLength, dataIn, 1024);

            Local<Array> data = Array::New(isolate);

            for(int i = 0; i < bitLength / 8; i++) {
                data->Set(i, Uint32::New(isolate, dataIn[i]));
            }
            

            // data->Set(0, Uint32::New(isolate, 8));
            // printf("Data: %d\n", dataIn[0]);

            info.GetReturnValue().Set(data);

        }

        static NAN_METHOD(getAdapters) {
            ec_adaptert * adapter = NULL;
            Isolate* isolate = info.GetIsolate();
            int counter = 0;
            Local<Array> data = Array::New(isolate);

            adapter = ec_find_adapters ();

            while (adapter != NULL)
            {
                // printf ("Description : %s, Device to use for wpcap: %s\n", adapter->desc,adapter->name);
                data->Set(counter, String::NewFromUtf8(isolate, adapter->name));
                adapter = adapter->next;
                counter++;
            }
            info.GetReturnValue().Set(data);
        }

        
        
            


        static inline Nan::Persistent<Function> & constructor() {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        };

        char *ifname_;
        char ioMap_[4096];
};

NODE_MODULE(objectwrapper, NodeSoemMaster::Init)
