#ifndef Menu_h
#define Menu_h

class Menu
{
public:
    Menu();
    void SetUp();
    void MainMenu();

private:
    int16_t leeEncoder();
    int8_t deltaEncoder();
    void inicializaLcd();
    void incializaRotaryEncoder();
    void escribeLcd(String mensaje1, String mensaje2);
    bool botonEncoderPulsado();
    float setValue(String, float, float, float, float);
    void UpdateData1();
    void UpdateData2();
    static void IRAM_ATTR readE();
    static void IRAM_ATTR readB();
    int16_t valEncoderAnt = 0;
    int16_t valEncoder = 0;
    int16_t current_pos = 0;
    int8_t delta_pos = 0;
    int low_limit = 0;
    int16_t upper_limit = 0;
    bool menu_changed = false;
    int16_t menu_pos = 0;  // To ndicate the current position inside the menu
    int16_t menus_pos = 0; // To indicate in which menu we are
};

#endif