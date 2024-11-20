#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>

#define MYSERVICE1_SERVICE_ID 0x1234
#define MYSERVICE1_INSTANCE_ID 0x5678
#define MYSERVICE1_METHOD_ID 0x0421


#define MYSERVICE2_SERVICE_ID 0x3489
#define MYSERVICE2_INSTANCE_ID 0x5678
#define MYSERVICE2_METHOD_ID 0x3456


static int count=0;
unsigned int flag = 0;
std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;
void run_MyService1();
void run_MyService2(); 
std::thread Mysender1(run_MyService1);
std::thread Mysender2(run_MyService2);
void on_availability_MyService1(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);

void run_MyService1() {
	std::cout <<"before lock\n";
    std::unique_lock<std::mutex> its_lock(mutex);
    condition.wait(its_lock);  // Wait until the service is available

	
	std::cout <<"Sending request\n";
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(MYSERVICE1_SERVICE_ID);
        request->set_instance(MYSERVICE1_INSTANCE_ID);
        request->set_method(MYSERVICE1_METHOD_ID);

        std::shared_ptr<vsomeip::payload> its_payload ;
        its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        for (vsomeip::byte_t i = 10; i < 20; i++) {
            its_payload_data.push_back(i % 256);
        }
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        app->send(request);
}


void run_MyService2() {

std::unique_lock<std::mutex> its_lock(mutex);
  condition.wait(its_lock);

  std::shared_ptr< vsomeip::message > request;
  request = vsomeip::runtime::get()->create_request();
  request->set_service(MYSERVICE2_SERVICE_ID);
  request->set_instance(MYSERVICE2_INSTANCE_ID);
  request->set_method(MYSERVICE2_METHOD_ID);

  std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
  std::vector< vsomeip::byte_t > its_payload_data;
  for (vsomeip::byte_t i=0; i<10; i++) {
      its_payload_data.push_back(i % 256);
  }
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  app->send(request);
}



void on_message_MyService1(const std::shared_ptr<vsomeip::message> &_request) {

  std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  // Get payload
  std::stringstream ss;
  for (vsomeip::length_t i=0; i<l; i++) {
     ss << std::setw(2) << std::setfill('0') << std::hex
        << (int)*(its_payload->get_data()+i) << " ";
  }

  std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
      << ss.str() << std::endl;
      
     
      // request
      
      app->request_service(MYSERVICE2_SERVICE_ID, MYSERVICE2_INSTANCE_ID);
      
      std::cout <<"Sending request\n";
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(MYSERVICE2_SERVICE_ID);
        request->set_instance(MYSERVICE2_INSTANCE_ID);
        request->set_method(MYSERVICE2_METHOD_ID);

        
        its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        for (vsomeip::byte_t i = 10; i < 20; i++) {
            its_payload_data.push_back(i % 256);
        }
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        app->send(request);
      
      
      
       
      
      
      
}

void on_message_MyService2(const std::shared_ptr<vsomeip::message> &_request) {

  std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  // Get payload
  std::stringstream ss;
  for (vsomeip::length_t i=0; i<l; i++) {
     ss << std::setw(2) << std::setfill('0') << std::hex
        << (int)*(its_payload->get_data()+i) << " ";
  }

  std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
      << ss.str() << std::endl;
      
      // request
      
      app->request_service(MYSERVICE1_SERVICE_ID, MYSERVICE1_INSTANCE_ID);
      
      std::cout <<"Sending request\n";
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(MYSERVICE1_SERVICE_ID);
        request->set_instance(MYSERVICE1_INSTANCE_ID);
        request->set_method(MYSERVICE1_METHOD_ID);

        
        its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        for (vsomeip::byte_t i = 10; i < 20; i++) {
            its_payload_data.push_back(i % 256);
        }
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        app->send(request);
}



void on_availability_MyService1(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {

	std::cout << "CLIENT: MyService1 ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;
    	if(_is_available){ 
    	
    	condition.notify_one();
    	std::cout <<"Unlocking thread\n";
    	}
}

void on_availability_MyService2(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {

  	std::cout << "CLIENT: MyService2 ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;
    	if(_is_available) condition.notify_one();
}




int main() {

  	app = vsomeip::runtime::get()->create_application("genx_MyClientApplication");
  	app->init();
  	app->register_availability_handler(MYSERVICE1_SERVICE_ID, MYSERVICE1_INSTANCE_ID, on_availability_MyService1);
	app->register_availability_handler(MYSERVICE2_SERVICE_ID, MYSERVICE2_INSTANCE_ID, on_availability_MyService2);
	
	app->register_message_handler(MYSERVICE1_SERVICE_ID, MYSERVICE1_INSTANCE_ID, MYSERVICE1_METHOD_ID, on_message_MyService1);
	app->register_message_handler(MYSERVICE2_SERVICE_ID, MYSERVICE2_INSTANCE_ID, MYSERVICE2_METHOD_ID, on_message_MyService2);
	
	app->request_service(MYSERVICE1_SERVICE_ID, MYSERVICE1_INSTANCE_ID);
	app->request_service(MYSERVICE2_SERVICE_ID, MYSERVICE2_INSTANCE_ID);
	
	
	
	
	app->start();
	
	
	
	
}
