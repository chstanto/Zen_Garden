/** @file EMDriver.h
 *  This class is responsible for setting up a driver to interact with the electromagnet
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

class EMDriver
{
    protected:
    public:
    EMDriver(uint8_t ENP);
    void enable(void);
    void disable(void);

    uint8_t EN;
};