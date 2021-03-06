#include "stdafx.h"
#include "ModInfo.h"
#include "Utility.h"
#include "ModEvents.h"
#include <util/Reflection.h>
#include <iostream>
#include <cstdint>

#include <event/game/FoliagePickupEvent.h>
#include <event/game/TakeDamageEvent.h>
#include <event/game/EnterChatMessageEvent.h>
#include <event/game/BeginPlayEvent.h>

void send_back(const Event& event, std::vector<void*>& data) {
	FARPROC proc = get_function((HMODULE)0x0000000180000000, "recieve_from_mod");
	auto pointer = (void(WINAPI*)(const Event&, std::vector<void*> data))proc;
	pointer(event, data);
}

// Custom Events
// - Create events you want to hook into the game here
// - To find out when events are called, please check the Event enum.
void OnPlayerTakenDamage(std::vector<void*>& args) {
	log_mod("Taken damage");
}

void OnPlayerChatMessageSent(std::vector<void*>& args) {
	std::string msg = *(std::string*)args[1];

	std::vector<std::string>* returns = (std::vector<std::string>*)args[2];

	if (msg == "/hello") {
		returns->push_back("Hello from Test Mod");
	}
}

EXTERN_DLL_EXPORT void setup() {
	dispatcher.subscribe(TakeDamageEvent::descriptor, OnPlayerTakenDamage);
	dispatcher.subscribe(EnterChatMessageEvent::descriptor, OnPlayerChatMessageSent);
}

EXTERN_DLL_EXPORT std::vector<void*> run_event(const Event& event, std::vector<void*>& args) {
	dispatcher.post(event, args);
	return args;
}