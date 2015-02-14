#ifndef SIGNAL_EMITTER_H
#define SIGNAL_EMITTER_H

class Player;
class Controller;
class Graphics;
class Camera;
class SignalReceiver;

class SignalEmitter
{
public:
	virtual ~SignalEmitter() {};

	virtual void update(const Player& player,
			    const Controller& controller) = 0;
	virtual void render(Graphics& graphics, const Camera& camera) = 0;

	virtual void addReceiver(SignalReceiver& target) = 0;
};

#endif /* SIGNAL_EMITTER_H */
