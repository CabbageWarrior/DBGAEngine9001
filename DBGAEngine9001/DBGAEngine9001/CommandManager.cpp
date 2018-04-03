#include "stdafx.h"
#include "CommandManager.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <iterator>
#include <SDL.h>
#include "Window.h"

#pragma region CommandManager
bool CommandManager::Inizialize()
{
	return false;
}
void CommandManager::Deinizialize()
{

}
bool CommandManager::ProcessCommand(const std::string & Command) const
{
	if (Command.empty())
	{
		return false;
	}

	std::map<std::string, CommandDefinition>::const_iterator commandElement = Commands.find(Command);
	if (commandElement == Commands.end()) {
		return false;
	}

	commandElement->second.CommandToExecute();
	return true;
}
bool CommandManager::AddCommand(const std::string & Command, const std::string & Description, void(*CommandFunction)(void))
{
	if (Command.empty() || !CommandFunction)
	{
		return false;
	}

	CommandDefinition MyCommand{ Command, Description, CommandFunction };

	// Ugly writing:
	//std::pair<std::map<std::string, CommandDefinition>::iterator, bool> returnValue = Commands.insert({ Command, MyCommand });
	//return returnValue.second;

	// Less ugly writing:
	return Commands.insert({ Command, MyCommand }).second;
}
std::map<std::string, std::string> CommandManager::GetCommands()
{
	std::map<std::string, std::string> commandsList;

	/*for (std::map<std::string, CommandDefinition>::iterator commandIterator = Commands.begin(); commandIterator != Commands.end(); ++commandIterator)
	{
		commandsList.insert(commandIterator->second.Name, commandIterator->second.Definition);
	}*/

	return commandsList;
}
#pragma endregion
