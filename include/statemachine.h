#include "types.h"

int high_score, current_score;
int player_healt_point;
bool paused;


class StateHandler {
public:
	/*! Функция осуществляет переход из состояния prev_state в состояние обработчиком которого является этот класс
	*	\param[in] prev_state состояние из которого нужно переходить
	*	\return истину если переход успешен.
	*/
	virtual bool _operator(int prev_state);
	StateHandler(bool (*function)(int));
	virtual ~StateHandler();
private:
	bool (*m_operator_inner)(int);
};

class StateMachine {
public:
	/*!	Функция задает обработчик для указанного состояния
	*	\param[in] state код задаваемого состояния, должен быть больше нуля
	*	\param[in] handler указатель на объект обработчик состояний
	*/
	void bindState(int state, StateHandler *handler);
	/*!	Функция осуществляет переход в указанное состояние
	*	\param[in] state состояние в которое перейти
	*	\return истину если переход успешен
	*/
	bool pushState(int state);
	StateMachine();
protected:
	int m_state;//code of state -1 if not state now, else positive number
	hhash<int, StateHandler*> m_state_list;
};