/** @file EMDriver.h
 *  This class is responsible for setting up a driver to interact with the electromagnet
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

/** @brief   Class for handling electromagnet
*/
class EMDriver
{
    protected:
    public:
    //Constructor
    EMDriver(uint8_t ENP);

    //Commands to turn on/off magnet
    void enable(void);
    void disable(void);

    //Enable pin
    uint8_t EN;
};