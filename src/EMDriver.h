class EMDriver
{
    protected:
    public:
    EMDriver(uint8_t ENP);
    void enable(void);
    void disable(void);

    uint8_t EN;
};