#include <iomanip>
#include <iostream>
#include <sstream>

#include <vsomeip/vsomeip.hpp>

#define MYSERVICE1_SERVICE_ID 0x1234
#define MYSERVICE1_INSTANCE_ID 0x5678
#define MYSERVICE1_METHOD_ID 0x0421




std::shared_ptr<vsomeip::application> app;

void on_message_MyService1(const std::shared_ptr<vsomeip::message> &_request) {

std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i=0; i<l; i++) {
       ss << std::setw(2) << std::setfill('0') << std::hex
          << (int)*(its_payload->get_data()+i) << " ";
    }

    std::cout << "SERVICE: Received message with Client/Session ["
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
        << ss.str() << std::endl;

    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (int i=19; i>=10; i--) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
    app->send(its_response);
}



int main() {

   app = vsomeip::runtime::get()->create_application("genx_MyServer1");
   app->init();
   app->register_message_handler(MYSERVICE1_SERVICE_ID,MYSERVICE1_INSTANCE_ID,MYSERVICE1_METHOD_ID,on_message_MyService1);
app->offer_service(MYSERVICE1_SERVICE_ID,MYSERVICE1_INSTANCE_ID);


   app->start();
}
