#pragma once

#include <string>
#include <map>
#include <list>

typedef void(*CommandFunction)(void);

struct CommandDefinition
{
	std::string Name = "";
	std::string Definition = "";
	CommandFunction CommandToExecute;
};

class CommandManager
{
	public:
		bool Inizialize();
		void Deinizialize();

		bool ProcessCommand(const std::string & Command) const;

		bool AddCommand(const std::string & Command, const std::string & Description, void(*CommandFunction)(void));

		std::map<std::string, std::string> GetCommands();
	private:
		std::map<std::string, CommandDefinition> Commands;
};
