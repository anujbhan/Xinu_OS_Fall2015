typedef struct queueNode{
	pid32 pid;
	struct queueNode* next;
} queueNode;


typedef struct queueFut{
	struct queueNode *head;
	struct queueNode *tail;
} queueFut;
