#ifndef SERVER_HANDLER_PHASE_EXIT
#define SERVER_HANDLER_PHASE_EXIT

void HandleExitPhase(int clientFD)
{
	HandleExitTeam(clientFD);

	auto accountID = Clients[clientFD];

	Accounts.erase(accountID);
	Clients.erase(clientFD);
}

#endif