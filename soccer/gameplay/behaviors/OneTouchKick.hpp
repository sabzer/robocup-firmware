#pragma once

#include "../Behavior.hpp"

#include <iostream>

namespace Gameplay
{
	class Window;
	class WindowEvaluator;

	namespace Behaviors
	{
		class OneTouchKick: public Behavior
		{
			public:
				OneTouchKick(GameplayModule *gameplay);
				~OneTouchKick();

				virtual void assign(std::set<Robot *> &available);
				virtual bool run();

				Robot *targetRobot;

				enum State
				{
					Intercept, // long distance, high speed
					Approach,  // final approach to get correct trajectory
					Done       // shot complete
				};
				State getState() {return _state;}

				/** Restarts the kick play - keep going after ball */
				void restart() {_state = Intercept;}

			protected:
				virtual float score(Robot* robot);

				WindowEvaluator *_win;

				State _state;

				// segment we are targetting
				Geometry2d::Segment _target;

				// Kick evaluation via obstacles
				Geometry2d::Segment evaluatePass(); /// finds a pass segment
				Geometry2d::Segment evaluateShot(); /// finds a shot segment


				// Individual state functions - these should execute a state's
				// activities, and return what the next state should be
				State intercept();
				State approach();

				// math functions
				int binomialCoefficient(int n, int i);
		};
	}
}
