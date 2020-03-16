/*
 * DIMMER CONTROL FOR 220V-AC-SCM-PWM
 * https://www.alibaba.com/product-detail/220V-AC-SCM-PWM-Dimming-Voltage_62452951439.html?spm=a2700.galleryofferlist.0.0.7b8c7a1a9Arg55
 *
 * It works via serial, baud 9600, receives messages to set the output:
 *  A220V  sets the output to 220v
 *  A120V sets the output to 120v
 */
#include "esphome.h"
#include "stdint.h"

/***********************GLOBAL VARIABLES*************************/
/*The variable that stores the brightness*/
uint16_t brightness = 220;
/*The constant that defines the amount of writes on the serial bus*/
const uint16_t POLLING_MS = 1000;

/*Class that emulates the light*/
class DimmerLight : public Component, public LightOutput
{
public:
    void setup() override
    {
        ESP_LOGD("DIMMER", "Light Initialized.");
    
    }

    LightTraits get_traits() override
    {
        // return the traits this light supports
        auto traits = LightTraits();
        traits.set_supports_brightness(true);
        traits.set_supports_rgb(false);
        traits.set_supports_rgb_white_value(false);
        traits.set_supports_color_temperature(false);
        return traits;
    }

    void write_state(LightState *state) override
    {
        // This will be called by the light to get a new state to be written.
        float brightness_float;
        // Brightness float is a value that goes from 0 to 1
        state->current_values_as_brightness(&brightness_float);
        brightness = (uint8_t)(brightness_float * 220);
        ESP_LOGD("DIMMER", "Update: %f -> %uV.", brightness_float, brightness);
    }
};

/*Class that continuosly write to the controller*/
class DimmerControl : public PollingComponent
{
public:
    Sensor *dummy= new Sensor();
    DimmerControl() : PollingComponent(POLLING_MS)
    {
        ESP_LOGD("DIMMER", "PollingComponent initialized.");
    }

    void setup() override
    {
        Serial.begin(9600);
        while (!Serial)
        {
        };
    }

    void update() override
    {
        printf("A%dV",brightness);
        ESP_LOGD("DIMMER", "WRITTEN: A%dV", brightness);
    }
};

