#include <deque>
#include <unordered_map>
#include <vector>
#include <list>
#include <mutex>

template <typename Hasher, typename Object> class AsyncHashMap {
public:std::unordered_map<Hasher, Object> HashMap;

	  bool count(Hasher Hash) {
		  bool out = false;
		  Mut.lock();
		  out = HashMap.count(Hash);
		  Mut.unlock();
		  return out;
	  }

	  void erase(Hasher Hash) {
		  Mut.lock();
		  HashMap.erase(Hash);
		  Mut.unlock();
	  }

	  Object get(Hasher Hash) {
		  Object Obj;
		  Mut.lock();
		  Obj = HashMap[Hash];
		  Mut.unlock();
		  return Obj;
	  }

	  Object* getAddress(Hasher Hash) {
		  return &HashMap[Hash];
	  }

	  template <typename Val, class... Args> void  RunObjFunction(Hasher Hash, Val Object::* Function, Args... args) { //pp
		  Mut.lock();
		  (&HashMap[Hash]->*Function)(args...);
		  Mut.unlock();
	  }

	  template <typename Val> void ChangeObjMember(Hasher Hash, Val Object::* Member, Val Value) { //took me so long to make this
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

	void pop_front() {
		Mut.lock();
		Vector.pop_front();
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
