
// A fifo queue implemented by a ring buffer.
template <typename DataType>
struct RingBuf {
	RingBuf(size_t storage_size)
		: storage(new DataType[storage_size]), storage_size(storage_size), used_start(0), used_size(0) {}
	~RingBuf() { delete[] storage; }

	DataType* push_elem() {
		if (used_size >= storage_size) {
			return nullptr;
		}
		return storage + ((used_start + used_size++) % storage_size);
	}

	DataType* get_next_elem() {
		if (used_size <= 0) {
			return nullptr;
		}
		return storage + used_start;
	}

	void pop_elem() {
		if (used_size <= 0) {
			return;
		}
		--used_size;
		used_start = (used_start + 1) % storage_size;
	}

	DataType* operator[](unsigned idx) {
		if (idx < used_start || idx >= used_size) {
			return nullptr;
		}
		return storage + ((used_start + idx) % storage_size);
	}

	/**
     * @return the number of elements currently available in the buffer.
     *    This is not the size of the storage buffer.
     */
	size_t size() { return used_size; }

   private:
	DataType* storage;
	size_t storage_size;
	unsigned used_start;
	size_t used_size;
};