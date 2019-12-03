#include "list.h"

bool listIsOrdered(List l) {
	
	Node curr = l->head;
	
	if (curr == NULL) {
		return true;
	}

	// flag to represent ascending/decending order
	// 1 for ascending, -1 for decending
	int flag = 0;

	while (curr->next != NULL) {
		// store the prev value for comparision
		int prev = curr->value;

		if (curr->next->value >= prev && flag != -1) {
			flag = 1;
		}
		else if (curr->next->value < prev && flag != 1) {
			flag = -1;
		}
		else {
			return false;
		}

		curr = curr->next;
	}
	return true;
}

