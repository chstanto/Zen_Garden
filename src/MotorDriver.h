class MotorDriver
{
    protected:
    public:
    MotorDriver(uint8_t motP1, uint8_t motP2, uint8_t ENP);
    void Enable_MOT(void);
    void Disable_MOT(void);
    void set_duty (float duty);

    uint8_t motorpin1;
    uint8_t motorpin2;
    uint8_t EN;
    float last_duty = 0;
};