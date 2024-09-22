
#ifndef _KERNEL_LIST_H
#define _KERNEL_LIST_H 1

// Doubly linked list
struct list_head {
	struct list_head* next;
	struct list_prev* prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)
#define INIT_LIST_HEAD(ptr) {(ptr)->next = (ptr); (ptr)->prev = (ptr);}

// Insert a new entry between two known consecutive entries
static inline void __list_add(
	struct list_head* new,
	struct list_head* prev,
	struct list_head* next
) {
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

// Delete list entry by making prev/next entries point to each other
static inline void __list_del(struct list_head* prev, struct list_head* next) {
	next->prev = prev;
	prev->next = next;
}



/**
 * @brief Add a new entry after entry to doubly linked list
 * 
 * Insert a new entry after the specified head. This is good for implementing
 * stacks.
 * 
 * @param new New entry to be added
 * @param head List head to add it after
*/
static inline void list_add(struct list_head* new, struct list_head* head) {
	__list_add(new, head, head->next);
}

/**
 * @brief Add a new entry before entry in doubly linked list
 * 
 * Insert a new entry before the specified head. This is useful for implementing
 * queues.
 * 
 * @param new New entry to be added
 * @param head List head to add it before
*/
static inline void list_add_tail(struct list_head* new, struct list_head* head) {
	__list_add(new, head->prev, head);
}

/**
 * @brief Delete entry from doubly linked list
 * 
 * Deletes an entry from the list by making the prev/next entries point to each
 * other.
 * 
 * @note list_empty on `entry` does not return true after this, the entry is in
 * an undefined state.
 * 
 * @param entry The element to delete from the list
*/
static inline void list_del(struct list_head* entry) {
	__list_del(entry->prev, entry->next);
}

/**
 * @brief Delete entry from doubly linked list, and reinitialise it.
 * 
 * Deletes an entry from the list by making the prev/next entries point to each
 * other. Reinitialises `entry` after it's removed.
 * 
 * @param entry The element to delete from the list
*/
static inline void list_del_init(struct list_head* entry) {
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * @brief Tests whether a doubly linked list is empty
 * 
 * Tests if a list is empty, i.e. the element links back to itself.
 * 
 * @param head The list to test
 * 
 * @returns Non-zero if list is emtpy.
*/
static inline int list_empty(struct list_head* head) {
	return head->next == head;
}

/**
 * @brief Join two lists
 * 
 * Joins two lists together
 * 
 * @param list The new list to add
 * @param head The place to add it in the first list
*/
static inline void list_splice(struct list_head* list, struct list_head* head) {
	struct list_head* new_list_head = list->next;

	// If not empty
	if (new_list_head != list) {
		struct list_head* new_list_tail = list->prev;
		struct list_head* after_head = head->next;

		// New list comes after head
		new_list_head->prev = head;
		head->next = new_list_head;

		// And everything after head comes after new list
		new_list_tail->next = after_head;
		after_head->prev = new_list_tail;
	}
}

/**
 * Get the struct for this entry
 * 
 * @param ptr The &struct list_head pointer
 * @param type The type of the struct this is embedded in
 * @param member The name of the list_struct within the struct
 * 
 * @todo More doc about this
*/
#define list_entry(ptr, type, member) ((type*)((char*)(ptr)-(unsigned long)(&((type*)0)->member)))

/**
 * Iterate over a list
 * 
 * @param pos The &struct list_head to use as a loop counter
 * @param head The head for your list
 * 
 * @todo More doc about this
*/
#define list_for_each(pos, head) for(pos = (head)->next; pos != (head); pos = pos->next)

#endif
