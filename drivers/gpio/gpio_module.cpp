/**
 * @file gpio_module.cpp
 *
 * @brief TODO.
 *
 * @details TODO. 
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 * @date 29/07/2024
 */

/* Abstraction-related headers */
#include "gpio_core.hpp"
#include "gpio_module.hpp"

drivers::GpioController::GpioController() {
    adapter_ = reinterpret_cast<gpio_adapter*>(match_adapter());
}
drivers::GpioController::~GpioController() {}

int drivers::GpioController::init(drivers::gpio_id pin) {
    if (!adapter_ || !adapter_->priv_data || !adapter_->init)
        return -1;

    adapter_->init(adapter_->priv_data, static_cast<unsigned int>(pin));

    return 0;
}
int drivers::GpioController::func(drivers::gpio_id pin, drivers::gpio_func func) {
    if (!adapter_ || !adapter_->priv_data || !adapter_->func)
        return -1;
    
    adapter_->func(adapter_->priv_data, static_cast<unsigned int>(pin), static_cast<gpio_function>(func));

    return 0;
}

int drivers::GpioController::dir(drivers::gpio_id pin, drivers::gpio_dir dir) {
    if (!adapter_ || !adapter_->priv_data || !adapter_->dir)
        return -1;

    adapter_->dir(adapter_->priv_data, static_cast<unsigned int>(pin), static_cast<bool>(dir));

    return 0;
}

int drivers::GpioController::set(drivers::gpio_id pin, drivers::gpio_state state) {
    if (!adapter_ || !adapter_->priv_data || !adapter_->set)
        return -1;

    adapter_->set(adapter_->priv_data, static_cast<unsigned int>(pin), static_cast<bool>(state));

    return 0;
}

bool drivers::GpioController::get(drivers::gpio_id pin) {
    if (!adapter_ || !adapter_->priv_data || !adapter_->get)
        return -1;

    return adapter_->get(adapter_->priv_data, static_cast<unsigned int>(pin));
}

int drivers::GpioController::toggle(drivers::gpio_id pin) {
    bool state = this->get(pin);
    if(state == true)
        adapter_->set(adapter_->priv_data, static_cast<unsigned int>(pin), static_cast<bool>(drivers::gpio_state::LOW));
    else
        adapter_->set(adapter_->priv_data, static_cast<unsigned int>(pin), static_cast<bool>(drivers::gpio_state::HIGH));

    return 0;
}


//!void GpioController::initPin(drivers::gpio_id pin) {
//!    gpio_init(static_cast<unsigned int>(pin));
//!}
//!void GpioController::setFunction(drivers::gpio_id pin, gpio_function function) {
//!    gpio_set_function(static_cast<unsigned int>(pin), function);
//!}
//!
//!void GpioController::setDirection(drivers::gpio_id pin, drivers::gpio_dir dir) {
//!    gpio_set_dir(static_cast<unsigned int>(pin), static_cast<bool>(dir));
//!}
//!
//!void GpioController::setState(drivers::gpio_id pin, drivers::gpio_state state) {
//!    gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(state));
//!}
//!
//!bool GpioController::getState(drivers::gpio_id pin) {
//!    return gpio_get_out_level(static_cast<unsigned int>(pin));
//!}
//!
//!void GpioController::togglePin(drivers::gpio_id pin) {
//!    bool state = getState(pin);
//!    if(state == true)
//!        gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(drivers::gpio_state::LOW));
//!    else
//!        gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(drivers::gpio_state::HIGH));
//!}
//!
