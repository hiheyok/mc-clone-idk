#pragma once

#include <deque>
#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>
#include <concurrent_priority_queue.h>
#include <concurrent_queue.h>

#include <vector>
#include <list>
#include <mutex>

template <typename Hasher, typename Object> class AsyncHashMapClass {
public:concurrency::concurrent_unordered_map<Hasher, Object> HashMap;

	  bool count(Hasher Hash) {
		  return HashMap.count(Hash);
	  }

	  bool empty() {
		  return HashMap.empty();
	  }

	  void erase(Hasher Hash) {
		  Mut.lock();
		  HashMap.unsafe_erase(Hash);
		  Mut.unlock();
	  }

	  void clear() {
		  HashMap.clear();
	  }

	  size_t size() {
		  return HashMap.size();
	  }

	  concurrency::concurrent_unordered_map<Hasher, Object> DumpData() {
		  return HashMap;
	  }

	  Object get(Hasher Hash) {
		  return HashMap[Hash];
	  }

	  Object* getAddress(Hasher Hash) {
		  return &HashMap[Hash];
	  }

	  template <typename Val, class... Args> void RunObjFunction(Hasher Hash, Val Object::* Function, Args... args) {
		  Mut.lock();
		  (&HashMap[Hash]->*Function)(args...);
		  Mut.unlock();
	  }

	  template <typename Val, class... Args> void RunObjFunctionReturn(Hasher Hash, Val Object::* Function, Val* ReturnAddress, Args... args) {
		  Mut.lock();
		  *ReturnAddress = (&HashMap[Hash]->*Function)(args...);
		  Mut.unlock();
	  }

	  template <typename Val> void ChangeObjMember(Hasher Hash, Val Object::* Member, Val Value) {
		  Mut.lock();
		  HashMap[Hash].*Member = Value;
		  Mut.unlock();
	  }

	  void insert(Hasher Hash, Object Obj) {
		  Mut.lock();
		  HashMap[Hash] = Obj;
		  Mut.unlock();
	  }

private:
	std::mutex Mut;
};

template <typename Hasher, typename Object> class AsyncHashMapNonClass {
public:Concurrency::concurrent_unordered_map<Hasher, Object> HashMap;

	  bool count(Hasher Hash) {
		  return HashMap.count(Hash);
	  }

	  bool empty() {
		  return HashMap.empty();
	  }

	  void erase(Hasher Hash) {
		  Mut.lock();
		  HashMap.unsafe_erase(Hash);
		  Mut.unlock();
	  }

	  void clear() {
		  HashMap.clear();
	  }

	  size_t size() {
		  return HashMap.size();
	  }

	  concurrency::concurrent_unordered_map<Hasher, Object> DumpData() {
		  return HashMap;
	  }

	  Object get(Hasher Hash) {
		  return HashMap[Hash];
	  }

	  Object* getAddress(Hasher Hash) {
		  return &HashMap[Hash];
	  }

	  void insert(Hasher Hash, Object Obj) {
		  Mut.lock();
		  HashMap[Hash] = Obj;
		  Mut.unlock();
	  }

private:
	std::mutex Mut;
};

template <typename Object> class AsyncDeque {
public:std::deque<Object> Deque;
	  size_t size() {
		  size_t size = 0;

		  Mut.lock();
		  size = Deque.size();
		  Mut.unlock();
		  return size;
	  }

	  bool empty() {
		  bool IsEmpty = false;
		  Mut.lock();
		  IsEmpty = Deque.empty();
		  Mut.unlock();
		  return IsEmpty;
	  }

	  void push_back(Object Obj) {
		  Mut.lock();
		  Deque.push_back(Obj);
		  Mut.unlock();
	  }

	  void push_front(Object Obj) {
		  Mut.lock();
		  Deque.push_front(Obj);
		  Mut.unlock();
	  }

	  void pop_back() {
		  Mut.lock();
		  Deque.push_back();
		  Mut.unlock();
	  }

	  void pop_front() {
		  Mut.lock();
		  Deque.pop_front();
		  Mut.unlock();
	  }

	  void clear() {
		  Mut.lock();
		  Deque.clear();
		  Mut.unlock();
	  }

	  void emplace_back(Object Obj) {
		  Mut.lock();
		  Deque.emplace_back(Obj);
		  Mut.unlock();
	  }

	  void emplace_front(Object Obj) {
		  Mut.lock();
		  Deque.emplace_front(Obj);
		  Mut.unlock();
	  }

	  Object front() {
		  Object Obj;
		  Mut.lock();
		  Obj = Deque.front();
		  Mut.unlock();
		  return Obj;
	  }

	  Object pop_get_front() {
		  Object Obj;
		  Mut.lock();
		  Obj = Deque.front();
		  Deque.pop_front();
		  Mut.unlock();
		  return Obj;
	  }

	  Object back() {
		  Object Obj;
		  Mut.lock();
		  Obj = Deque.back();
		  Mut.unlock();
		  return Obj;
	  }
	  Object pop_get_back() {
		  Object Obj;
		  Mut.lock();
		  Obj = Deque.back();
		  Deque.pop_back();
		  Mut.unlock();
		  return Obj;
	  }

private:
	std::mutex Mut;
};

template <typename Object> class AsyncVector {
public:std::vector<Object> Vector;
	size_t size() {
		size_t size = 0;

		Mut.lock();
		size = Vector.size();
		Mut.unlock();
		return size;
	}

	bool empty() {
		bool IsEmpty = false;
		Mut.lock();
		IsEmpty = Vector.empty();
		Mut.unlock();
		return IsEmpty;
	}

	void push_back(Object Obj) {
		Mut.lock();
		Vector.push_back(Obj);
		Mut.unlock();
	}

	void pop_back() {
		Mut.lock();
		Vector.push_back();
		Mut.unlock();
	}

	void clear() {
		Mut.lock();
		Vector.clear();
		Mut.unlock();
	}

	void emplace_back(Object Obj) {
		Mut.lock();
		Vector.emplace_back(Obj);
		Mut.unlock();
	}

	void emplace_front(Object Obj) {
		Mut.lock();
		Vector.emplace_front(Obj);
		Mut.unlock();
	}

	Object front() {
		Object Obj;
		Mut.lock();
		Obj = Vector.front();
		Mut.unlock();
		return Obj;
	}

	Object back() {
		Object Obj;
		Mut.lock();
		Obj = Vector.back();
		Mut.unlock();
		return Obj;
	}

private:
	std::mutex Mut;
};
