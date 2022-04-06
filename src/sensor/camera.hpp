#pragma once

class Camera {
    bool recording;
    int trigger_pin;

    static constexpr int start_delay = 600;
    static constexpr int stop_delay = 600;

    void pulse_low(const int duration);

public:
    Camera(int);
    void setup_pins();

    void start_recording();
    void stop_recording();
};
