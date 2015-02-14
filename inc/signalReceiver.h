#ifndef SIGNAL_RECEIVER_H
#define SIGNAL_RECEIVER_H

class Player;
class Controller;
class Graphics;
class Camera;

class SignalReceiver
{
public:
	virtual ~SignalReceiver() {};

	virtual void update(Player& player,
			    const Controller& controller) = 0;
	virtual void render(Graphics& graphics, const Camera& camera) = 0;

	virtual void sendSignal() = 0;
};

#endif /* SIGNAL_RECEIVER_H */
