#include "Racetrack.h"
#include <iostream>
#include <string>

Racetrack::Racetrack() : m_Track()
{
	// Let the initializer list construct the m_Track vector
	// m_Weights can not be created until the size of the m_Track is known
	m_Weights = nullptr;
}

/**
 *Destroy object and m_Weights pointer
 */
Racetrack::~Racetrack()
{
	// I can not delete an array if it has not yet been created
	if (m_Weights != nullptr)
	{
		// delete each row
		for (int i = 0; i < this->Height(); i++)
		{
			delete[] m_Weights[i];
		}

		// delete the array of pointers
		delete[] m_Weights;
	}
}

/**
 *Read m_Track and add elements to vector and m_Weights to weight array
 */
void Racetrack::Read(std::istream& ins)
{
	std::string element;
	while (ins >> element)
	{
		for (int i = 0; i < element.size(); i++)
		{
			// Replace the T's with blank/m_Track spaces
			element = ReplaceStrChar(element, "T", ' ');
		}
		// Add current element to the m_Track vector
		m_Track.push_back(element);
	}
	// Initialize m_Weights after m_Track has been created
	this->InitWeights();
}

/**
 *Convert string into character
 */
std::string Racetrack::ReplaceStrChar(std::string str,
		const std::string& replace, char ch)
{
	// set our locator equal to the first appearance of any character in replace
	size_t found = str.find_first_of(replace);

	while (found != std::string::npos)
	{
		// While our position in the sting is in range.
		str[found] = ch; // Change the character at position.
		found = str.find_first_of(replace, found + 1); // Relocate again.
	}

	return str; // return our new string.
}

/**
 *Access an element from the m_Track
 */
char Racetrack::GetTrack(int y, int x)
{
	return m_Track[y][x];
}

/**
 *Update an element from the m_Track
 */
void Racetrack::SetTrack(int y, int x, char value)
{
	m_Track[y][x] = value;
}

/**
 *Get value of weight location
 **/
int Racetrack::GetWeight(int y, int x)
{
	return m_Weights[y][x];
}

/**
 *Update a weight from the weight m_Track
 */
void Racetrack::SetWeight(int y, int x, int value)
{
	m_Weights[y][x] = value;
}

/**
 *Displays Track in Game Mode
 */
void Racetrack::DisplayTrack()
{
	for (int i = 0; i < Height(); i++)
	{
		for (int j = 0; j < Width(); j++)
		{
			std::cout << std::setw(3) << m_Track[i][j];
		}
		std::cout << std::endl;
	}
}

/**
 *Displays the m_Weights of the current m_Track.  Used for Testing/Debugging
 *Purposes
 */
void Racetrack::DisplayWeights()
{
	std::cout << "********************TRACK WEIGHTS***********************"
			  << std::endl;
	for (int i = 0; i < Height(); i++)
	{
		for (int j = 0; j < Width(); j++)
		{
			if (m_Weights[i][j] > 100)
			{
				std::cout << std::setw(3) << "X";
			}
			else
			{
				std::cout << std::setw(3) << m_Weights[i][j];
			}
		}
		std::cout << std::endl;
	}
	std::cout << "********************TRACK WEIGHTS***********************"
			  << std::endl
			  << std::endl;
}

/**
 *Initialize weight values
 */
void Racetrack::InitWeights()
{
	// Allocate the number of required rows (pointers)
	m_Weights = new int* [Height()];

	// Allocate each row
	for (int i = 0; i < Height(); i++)
	{
		m_Weights[i] = new int[Width()];

		// Set each cell as uninitialized
		for (int j = 0; j < Width(); j++)
		{
			m_Weights[i][j] = m_W_UNINIT;

			// Set wall m_Weights
			if (toupper(m_Track[i][j]) == 'X')
			{
				m_Weights[i][j] = m_W_WALL;
			}
			else if (toupper(m_Track[i][j]) == 'F')
			{
				// Set finish line weight
				m_Weights[i][j] = m_W_FINISH;
			}
		}
	}

	// Weight initialization algorithm.
	int currentWeight = m_W_FINISH;
	while (true)
	{
		bool found = false;
		for (int row = 0; row < Height(); row++)
		{
			for (int column = 0; column < Width(); column++)
			{
				if (m_Weights[row][column] == currentWeight)
				{
					for (int i = row - 1; i <= row + 1; i++)
					{
						for (int j = column - 1; j <= column + 1; j++)
						{
							if (m_Weights[i][j] == m_W_UNINIT)
							{
								m_Weights[i][j] = currentWeight + 1;
								found = true;
							}
						}
					}
				}
			}
		}
		if (!found)
		{
			break;
		}
		currentWeight += 1;
	}
}

void Racetrack::PutCarsOntoTrack()
{
	int highestUserWeight = 0;
	int highestSpeedWeight = 0;
	int highestHandleWeight = 0;

	// Place user car onto the track.
	for (int row = 0; row < Height(); row++)
	{
		for (int column = 0; column < Width(); column++)
		{
			if (m_Weights[row][column] > highestUserWeight &&
				m_Weights[row][column] != m_W_WALL)
			{
				highestUserWeight = m_Weights[row][column];
				user.SetRowNumber(row);
				user.SetColumnNumber(column);
			}
		}
	}
	m_Track[user.GetRowNumber()][user.GetColumnNumber()] = user.GetIDNumber();

	// Place speed car onto the track.
	for (int row = 0; row < Height(); row++)
	{
		for (int column = 0; column < Width(); column++)
		{
			if (m_Weights[row][column] > highestSpeedWeight &&
				m_Weights[row][column] != m_W_WALL &&
				m_Track[row][column] == ' ')
			{
				highestSpeedWeight = m_Weights[row][column];
				speed.SetRowNumber(row);
				speed.SetColumnNumber(column);
			}
		}
	}
	m_Track[speed.GetRowNumber()][speed.GetColumnNumber()] = speed.GetIDNumber();

	// Place handle car onto the track.
	for (int row = 0; row < Height(); row++)
	{
		for (int column = 0; column < Width(); column++)
		{
			if (m_Weights[row][column] > highestHandleWeight &&
				m_Weights[row][column] != m_W_WALL &&
				m_Track[row][column] == ' ')
			{
				highestHandleWeight = m_Weights[row][column];
				handle.SetRowNumber(row);
				handle.SetColumnNumber(column);
			}
		}
	}
	m_Track[handle.GetRowNumber()][handle.GetColumnNumber()] =
			handle.GetIDNumber();
}

void Racetrack::MoveUserCar()
{
	// Get the users current coordinates
	float oldYCoordinate = float(user.GetRowNumber());
	float oldXCoordinate = float(user.GetColumnNumber());
	int yCoordinate = 0;
	int xCoordinate = 0;
	std::vector<int> lastRows;
	std::vector<int> lastColumns;

	std::cout << "Enter X Coordinate: ";
	while (!(std::cin >> xCoordinate) ||
		   xCoordinate > user.GetColumnVelocity() ||
		   xCoordinate < -(user.GetColumnVelocity()))
	{
		std::cout << "Please enter a valid number: ";
		std::cin.clear();
		std::cin.ignore(123, '\n');
	}

	// As long as the user's input is not 0, and their velocity hasn't reached its
	// max, the column velocity will increase by 1.
	if (xCoordinate != 0)
	{
		if (user.GetColumnVelocity() < user.GetMaxSpeed())
		{
			user.SetColumnVelocity(user.GetColumnVelocity() + 1);
		}
	}

	std::cout << "Enter Y Coordinate: ";
	while (!(std::cin >> yCoordinate) ||
		   yCoordinate > user.GetRowVelocity() ||
		   yCoordinate < -(user.GetRowVelocity()))
	{
		std::cout << "Please enter a valid number: ";
		std::cin.clear();
		std::cin.ignore(123, '\n');
	}

	// As long as the user's input is not 0, and their velocity hasn't reached its
	// max, the row velocity will increase by 1.
	if (yCoordinate != 0)
	{
		if (user.GetRowVelocity() < user.GetMaxSpeed())
		{
			user.SetRowVelocity(user.GetRowVelocity() + 1);
		}
	}

	// Update new positions based on the coordinates the user typed in.
	int newYCoordinate = user.GetRowNumber() + yCoordinate;
	int newXCoordinate = user.GetColumnNumber() + xCoordinate;
	float ySlope = (oldYCoordinate - float(newYCoordinate)) / 10;
	float xSlope = (oldXCoordinate - float(newXCoordinate)) / 10;
	bool hitSomething = false;

	// Check the user car's path.
	for (int i = 0; i < 10; i++)
	{
		oldYCoordinate = oldYCoordinate - ySlope;
		oldXCoordinate = oldXCoordinate - xSlope;
		lastRows.push_back(round(oldYCoordinate));
		lastColumns.push_back(round(oldXCoordinate));

		// Check for collisions and if the user passed the finish line.
		bool checkFinish =
				m_Weights[int(round(oldYCoordinate))][int(
						round(oldXCoordinate))] ==
				m_W_FINISH;
		bool checkWall =
				m_Weights[int(round(oldYCoordinate))][int(
						round(oldXCoordinate))] ==
				m_W_WALL;
		bool speedCollision =
				m_Track[int(round(oldYCoordinate))][int(
						round(oldXCoordinate))] ==
				m_Track[speed.GetRowNumber()][speed.GetColumnNumber()];
		bool handleCollision =
				m_Track[int(round(oldYCoordinate))][int(
						round(oldXCoordinate))] ==
				m_Track[handle.GetRowNumber()][handle.GetColumnNumber()];

		// Check to see if the user has won
		if (checkFinish)
		{
			WinningScreen();
			DeleteOldUserCar();
			UpdateUserPosition(yCoordinate, xCoordinate);
			DisplayTrack();
			std::cin.ignore();
			std::cin.get();
			exit(0);
		}
		else if (checkWall)
		{
			// Notifies the user they hit a wall, resets their velocities,
			// and lowers their max speed by 1
			std::cout
					<< "You hit a wall! Your velocity has been reset and max speed "
					   "has been decreased."
					<< std::endl;
			xCoordinate = 0;
			yCoordinate = 0;
			if (user.GetMaxSpeed() != 1)
			{
				user.SetMaxSpeed(user.GetMaxSpeed() - 1);
			}
			user.SetColumnVelocity(1);
			user.SetRowVelocity(1);
			hitSomething = true;
			break;
		}
		else if (speedCollision || handleCollision)
		{
			// Notifies the user they hit another car, resets their velocities,
			// and lowers their max speed by 1
			std::cout
					<< "You hit a car! Your velocity has been reset and max speed "
					   "has been decreased."
					<< std::endl;
			xCoordinate = 0;
			yCoordinate = 0;
			if (user.GetMaxSpeed() != 1)
			{
				user.SetMaxSpeed(user.GetMaxSpeed() - 1);
			}
			user.SetColumnVelocity(1);
			user.SetRowVelocity(1);
			hitSomething = true;
			break;
		}
	}

	// Moves the car to the position right before it made a collision.
	lastRows.erase(std::unique(lastRows.begin(), lastRows.end()), lastRows.end());
	lastColumns.erase(std::unique(lastColumns.begin(), lastColumns.end()), lastColumns.end());
	if (hitSomething && lastRows.size() > 1 && lastColumns.size() > 1)
	{
		lastRows.pop_back();
		lastColumns.pop_back();
		DeleteOldUserCar();
		user.SetRowNumber(lastRows.back());
		user.SetColumnNumber(lastColumns.back());
		yCoordinate = 0;
		xCoordinate = 0;
	}

	DeleteOldUserCar();
	UpdateUserPosition(yCoordinate, xCoordinate);
}

void Racetrack::MoveCPUSpeedCar()
{
	float currentYPosition = float(speed.GetRowNumber());
	float currentXPosition = float(speed.GetColumnNumber());
	int oldYPosition = speed.GetRowNumber();
	int oldXPosition = speed.GetColumnNumber();
	int newYPosition = 0;
	int newXPosition = 0;
	int lowestNumber = m_Weights[int(currentYPosition)][int(currentXPosition)];
	bool hitSomething = false;
	std::vector<int> lastRows;
	std::vector<int> lastColumns;

	// Look at the elements around the car.
	for (int row = int(currentYPosition) - speed.GetRowVelocity();
		 row <= int(currentYPosition) + speed.GetRowVelocity(); row++)
	{
		for (int column = int(currentXPosition) - speed.GetColumnVelocity();
			 column <= int(currentXPosition) + speed.GetColumnVelocity();
			 column++)
		{
			// Skip over the Speed Car and just look at the surrounding elements.
			if (row < 0 || row >= Height() || column < 0 || column >= Width() ||
				(row == int(currentYPosition) &&
				 column == int(currentXPosition)))
			{
				continue;
			}

			if (m_Weights[row][column] < lowestNumber)
			{
				lowestNumber = m_Weights[row][column];
				newYPosition = row;
				newXPosition = column;
			}
		}
	}

	float ySlope = (currentYPosition - float(newYPosition)) / 10;
	float xSlope = (currentXPosition - float(newXPosition)) / 10;

	for (int i = 0; i < 10; i++)
	{
		currentYPosition = currentYPosition - ySlope;
		currentXPosition = currentXPosition - xSlope;
		lastRows.push_back(round(currentYPosition));
		lastColumns.push_back(round(currentXPosition));

		bool checkFinish =
				m_Weights[int(round(currentYPosition))][int(
						round(currentXPosition))] ==
				m_W_FINISH;
		bool checkOldWall =
				m_Weights[int(round(currentYPosition))][int(
						round(currentXPosition))] ==
				m_W_WALL;
		bool checkNewWall =
				m_Weights[int(round(newYPosition))][int(round(newXPosition))] ==
				m_W_WALL;
		bool userOldCollision =
				m_Track[int(round(currentYPosition))][int(
						round(currentXPosition))] ==
				m_Track[user.GetRowNumber()][user.GetColumnNumber()];
		bool handleOldCollision =
				m_Track[int(round(currentYPosition))][int(
						round(currentXPosition))] ==
				m_Track[handle.GetRowNumber()][handle.GetColumnNumber()];
		bool userNewCollision =
				m_Track[int(round(newYPosition))][int(round(newXPosition))] ==
				m_Track[user.GetRowNumber()][user.GetColumnNumber()];
		bool handleNewCollision =
				m_Track[int(round(newYPosition))][int(round(newXPosition))] ==
				m_Track[handle.GetRowNumber()][handle.GetColumnNumber()];

		if (checkFinish)
		{
			LosingScreen();
			DeleteOldSpeedCar();
			UpdateSpeedPosition(newYPosition, newXPosition);
			DisplayTrack();
			std::cin.ignore();
			std::cin.get();
			exit(0);
		}
		else if (checkOldWall || checkNewWall)
		{
			std::cout
					<< "Car 2 hit a wall! Their velocity has been reset and max speed "
					   "has been decreased."
					<< std::endl;
			if (speed.GetMaxSpeed() != 1)
			{
				speed.SetMaxSpeed(speed.GetMaxSpeed() - 1);
			}
			speed.SetColumnVelocity(1);
			speed.SetRowVelocity(1);
			hitSomething = true;
			break;
		}
		else if (userOldCollision || handleOldCollision || userNewCollision ||
				 handleNewCollision)
		{
			std::cout
					<< "Car 2 hit a car! Their velocity has been reset and max speed "
					   "has been decreased."
					<< std::endl;
			if (speed.GetMaxSpeed() != 1)
			{
				speed.SetMaxSpeed(speed.GetMaxSpeed() - 1);
			}
			speed.SetColumnVelocity(1);
			speed.SetRowVelocity(1);
			hitSomething = true;
			break;
		}
	}

	// Moves the car to the position right before it made a collision.
	lastRows.erase(std::unique(lastRows.begin(), lastRows.end()), lastRows.end());
	lastColumns.erase(std::unique(lastColumns.begin(), lastColumns.end()), lastColumns.end());
	if (hitSomething && lastRows.size() > 1 && lastColumns.size() > 1)
	{
		lastRows.pop_back();
		lastColumns.pop_back();
		DeleteOldSpeedCar();
		UpdateSpeedPosition(lastRows.back(), lastColumns.back());
	}
	else if (!hitSomething)
	{
		// As long as the speed car's newYPosition is not 0, and their velocity
		// hasn't reached its max, the row velocity will increase by 1.
		if (abs(newYPosition - int(oldYPosition)) > 0)
		{
			if (speed.GetRowVelocity() < speed.GetMaxSpeed())
			{
				speed.SetRowVelocity(speed.GetRowVelocity() + 1);
			}
		}

		// As long as the speed car's newXPosition is not 0, and their velocity
		// hasn't reached its max, the column velocity will increase by 1.
		if (abs(newXPosition - int(oldXPosition)) > 0)
		{
			if (speed.GetColumnVelocity() < speed.GetMaxSpeed())
			{
				speed.SetColumnVelocity(speed.GetColumnVelocity() + 1);
			}
		}

		DeleteOldSpeedCar();
		UpdateSpeedPosition(newYPosition, newXPosition);
	}
}

void Racetrack::MoveCPUHandleCar()
{
	int oldYPosition = handle.GetRowNumber();
	int oldXPosition = handle.GetColumnNumber();
	int newYPosition = 0;
	int newXPosition = 0;
	int lowestNumber = 100;

	// Look at the elements around the car.
	for (int row = int(oldYPosition) - handle.GetRowVelocity();
		 row <= int(oldYPosition) + handle.GetRowVelocity(); row++)
	{
		for (int column = int(oldXPosition) - handle.GetColumnVelocity();
			 column <= int(oldXPosition) + handle.GetColumnVelocity(); column++)
		{
			// Skip over the Handle Car and just look at the surrounding elements.
			if (row < 0 || row >= Height() || column < 0 || column >= Width() ||
				(row == int(oldYPosition) && column == int(oldXPosition)))
			{
				continue;
			}

			float ySlope = (float(oldYPosition) - float(row)) / 10;
			float xSlope = (float(oldXPosition) - float(column)) / 10;
			float currentYPosition = float(oldYPosition);
			float currentXPosition = float(oldXPosition);
			bool hitSomething = false;

			// Path checking.
			for (int i = 0; i < 10; i++)
			{
				currentYPosition = currentYPosition - ySlope;
				currentXPosition = currentXPosition - xSlope;

				// Conditions to check collisions (Looks cleaner in the if statements
				// to just have the name instead of the logic)
				bool checkOldWall =
						m_Weights[int(round(oldYPosition))][int(
								round(oldXPosition))] ==
						m_W_WALL;
				bool checkNewWall = m_Weights[int(round(currentYPosition))]
									[int(round(currentXPosition))] == m_W_WALL;
				bool userNewCollision =
						m_Track[int(round(currentYPosition))]
						[int(round(currentXPosition))] ==
						m_Track[user.GetRowNumber()][user.GetColumnNumber()];
				bool speedNewCollision =
						m_Track[int(round(currentYPosition))]
						[int(round(currentXPosition))] ==
						m_Track[speed.GetRowNumber()][speed.GetColumnNumber()];
				bool userOldCollision =
						m_Track[int(round(oldYPosition))][int(
								round(oldXPosition))] ==
						m_Track[user.GetRowNumber()][user.GetColumnNumber()];
				bool speedOldCollision =
						m_Track[int(round(oldYPosition))][int(
								round(oldXPosition))] ==
						m_Track[speed.GetRowNumber()][speed.GetColumnNumber()];

				if (userOldCollision || speedOldCollision || userNewCollision ||
					speedNewCollision || checkOldWall || checkNewWall)
				{
					hitSomething = true;
				}
			}

			if (hitSomething)
			{
				continue;
			}

			// Save the lowest weight that hasn't collided with something.
			if (m_Weights[row][column] < lowestNumber)
			{
				lowestNumber = m_Weights[row][column];
				newYPosition = row;
				newXPosition = column;
			}
		}
	}

	bool checkFinish =
			m_Weights[int(round(newYPosition))][int(round(newXPosition))] ==
			m_W_FINISH;

	if (checkFinish)
	{
		LosingScreen();
		DeleteOldHandleCar();
		UpdateHandlePosition(newYPosition, newXPosition);
		DisplayTrack();
		std::cin.ignore();
		std::cin.get();
		exit(0);
	}

	// As long as the handle car's newYPosition is not 0, and their velocity
	// hasn't reached its max, the row velocity will increase by 1.
	if (abs(newYPosition - int(oldYPosition)) > 0)
	{
		if (handle.GetRowVelocity() < handle.GetMaxSpeed())
		{
			handle.SetRowVelocity(handle.GetRowVelocity() + 1);
		}
	}

	// As long as the handle car's newXPosition is not 0, and their velocity
	// hasn't reached its max, the column velocity will increase by 1.
	if (abs(newXPosition - int(oldXPosition)) > 0)
	{
		if (handle.GetColumnVelocity() < handle.GetMaxSpeed())
		{
			handle.SetColumnVelocity(handle.GetColumnVelocity() + 1);
		}
	}

	DeleteOldHandleCar();
	UpdateHandlePosition(newYPosition, newXPosition);
}

void Racetrack::UpdateUserPosition(int yCoord, int xCoord)
{
	user.SetRowNumber(user.GetRowNumber() + yCoord);
	user.SetColumnNumber(user.GetColumnNumber() + xCoord);
	m_Track[user.GetRowNumber()][user.GetColumnNumber()] = user.GetIDNumber();
}

void Racetrack::DeleteOldUserCar()
{
	m_Track[user.GetRowNumber()][user.GetColumnNumber()] = ' ';
}

void Racetrack::DisplayAllStats()
{
	user.DisplayStats();
	speed.DisplayStats();
	handle.DisplayStats();
}

void Racetrack::WinningScreen()
{
	std::cout << "██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗ ██████╗ ███╗   ██╗"
			  << std::endl;
	std::cout << "╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██╔═══██╗████╗  ██║"
			  << std::endl;
	std::cout << "  ████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║   ██║██╔██╗ ██║"
			  << std::endl;
	std::cout << "  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║   ██║██║╚██╗██║"
			  << std::endl;
	std::cout << "   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝╚██████╔╝██║ ╚████║"
			  << std::endl;
	std::cout << "   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═══╝"
			  << std::endl;
}

void Racetrack::UpdateSpeedPosition(int newYCoord, int newXCoord)
{
	speed.SetRowNumber(newYCoord);
	speed.SetColumnNumber(newXCoord);
	m_Track[newYCoord][newXCoord] = speed.GetIDNumber();
}

void Racetrack::DeleteOldSpeedCar()
{
	m_Track[speed.GetRowNumber()][speed.GetColumnNumber()] = ' ';
}

void Racetrack::LosingScreen()
{
	std::cout << " ▄· ▄▌      ▄• ▄▌  ▄▄▌        .▄▄ · ▄▄▄▄▄▄" << std::endl;
	std::cout << "▐█▪██▌ ▄█▀▄ █▪██▌  ██•   ▄█▀▄ ▐█ ▀. ▀•██ ▀" << std::endl;
	std::cout << "▐█▌▐█▪▐█▌.▐▌█▌▐█▌  ██ ▪ ▐█▌.▐▌▄▀▀▀█▄  ▐█.▪" << std::endl;
	std::cout << " ▐█▀·.▐█▌.▐▌▐█▄█▌  ▐█▌ ▄▐█▌.▐▌▐█▄▪▐█  ▐█▌·" << std::endl;
	std::cout << "  ▀ •  ▀█▄▀▪ ▀▀▀   .▀▀▀  ▀█▄▀▪ ▀▀▀▀   ▀▀▀" << std::endl;
}

void Racetrack::UpdateHandlePosition(int newYCoord, int newXCoord)
{
	handle.SetRowNumber(newYCoord);
	handle.SetColumnNumber(newXCoord);
	m_Track[newYCoord][newXCoord] = handle.GetIDNumber();
}

void Racetrack::DeleteOldHandleCar()
{
	m_Track[handle.GetRowNumber()][handle.GetColumnNumber()] = ' ';
}
