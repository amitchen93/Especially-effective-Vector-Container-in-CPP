//
// Created by amitc on 04/08/2020.
//

#ifndef EXAM_CPP_VLVECTOR_HPP
#define EXAM_CPP_VLVECTOR_HPP
#include <algorithm>

/**
 * @brief class for exception to throw when you ask to
 */
class Error_INDEX : public std::exception // VALID LIBRARY BY YOUR RULES
{
    const char *what() const noexcept override { return "Invalid Index" ; }
};

template<class T, unsigned int StaticCapacity = 16>
/**
 * @brief class of out VLV!
 * @tparam T type
 * @tparam StaticCapacity Static Capacity
 */
class VLVector  // it's not regular class -> it's a template of creating classes of Vectors
{
private:
    T _vStack[StaticCapacity]; // vector of T type with StaticCapacity capacity
    T *_vHeap; // vector of T type with dynamic capacity
    int sizeOfVLVector;  // _curr sizeOfVLVector of vector (num of exist elements)
    int capacityOfVLVector; // _curr capacityOfVLVector of vector (how many elements it can hold)
    bool trueIfHeap = false; // a switch represent true=heap, false=stack


    /**
     * @brief calc new Capacity of VLVector by given formula
     * @return int represent new Capacity of VLVector
     */
    int _newCapacityMethod()
    {
        return (3 * (sizeOfVLVector + 1) / 2);
    }

    /**
     * @brief move vector from stack to heap
     * @return vector in heap
     */
    void _moveToHeap()
    {
        int newCapacityOfVLVector = _newCapacityMethod();
        this->_vHeap = new T[newCapacityOfVLVector]; // create struct of T with _newCapacityMethod
        for (int i = 0; i < sizeOfVLVector; ++i)
        {
            _vHeap[i] = _vStack[i];
        }
        this->capacityOfVLVector = newCapacityOfVLVector;  // update capacity of VLVector
        this->trueIfHeap = true;  // we move to heap so it's true
    }

    /**
     * @brief increase heap array to bigger one if we have to
     * @return vector in heap
     */
    void _increaseHeap()
    {
        int newCapacityOfVLVector = _newCapacityMethod();
        T *tmpVLV = new T[newCapacityOfVLVector]; // create bigger array
        for (int i = 0; i < sizeOfVLVector; ++i)  // copy old values
        {
            tmpVLV[i] = _vHeap[i];
        }
        delete[] (_vHeap); // free old dynamic array
        _vHeap = tmpVLV; // point to tmpVLV (bigger array)
        this->capacityOfVLVector = newCapacityOfVLVector; // update capacity of VLVector
    }

    /**
     * @brief move vector from heap to stack
     * @return vector in stack
     */
    void _moveToStack()
    {
        for (int i = 0; i < (int)StaticCapacity; ++i)
        {
            _vStack[i] = _vHeap[i];
            _vHeap[i].~T();
        }
        delete[] _vHeap;
        this->capacityOfVLVector = StaticCapacity;  // update capacity of VLVector
        this->trueIfHeap = false;  // we move to stack so it's false
    }

public:
    /** constructors */
    /// Default constructor
    VLVector() : _vHeap(nullptr), sizeOfVLVector(0),
                 capacityOfVLVector(StaticCapacity), trueIfHeap(false){}


    /// constructor 1
    /**
     * @brief create new VLV and put the element from iterator to it
     */
    template<class InputIterator> VLVector(InputIterator first, InputIterator last)
    {

        this->_vHeap = nullptr;
        this->sizeOfVLVector = 0;
        this->capacityOfVLVector = StaticCapacity;
        this->trueIfHeap = false;

        for (InputIterator it = first ; it < last ; it ++)
        {
            this->push_back((*it));
        }
    }

    /**
     * @brief copy constructor - update primitive fields of VLV with initialization list
     */
    VLVector(VLVector & rhs) : _vHeap(nullptr),
                               sizeOfVLVector(rhs.sizeOfVLVector),
                               capacityOfVLVector(rhs.capacityOfVLVector),
                               trueIfHeap(rhs.trueIfHeap)
    {
        iterator it = rhs.begin();      // create iterator
        // case 1 : create stack
        if ((!rhs.trueIfHeap))          // we need to create stack VLV
        {
            for (int index = 0 ; index < rhs.size() ; index ++)
            {
                this->at(index) = rhs.at(index);
            }
        }
            // case 2 : to heap
        else if (rhs.trueIfHeap) // we need to create heap VLV
        {
            this->_vHeap = new T[rhs.capacity()];        // alloc new memory to new dynamic VLV
            for (int index = 0; index < rhs.size() ; index ++)
            {
                this->at(index) = rhs.at(index);
            }
        }
    } // similar to TA4?

    /// destructor
    ~VLVector()
    {
        if (this->trueIfHeap) // we on heap
        {
            delete [] this->_vHeap;
        }
        this->sizeOfVLVector = 0;
        this->capacityOfVLVector = 0;
    }


    /** operators */
    VLVector<T, StaticCapacity> &operator=(VLVector<T, StaticCapacity> &rhs);

    T &operator[](int i) noexcept;

    const T &operator[](int i) const noexcept ;

    bool operator==(VLVector<T, StaticCapacity> &rhs)
    {
        // check size and capacity of both of them
        if ((this->size() != rhs.size()) || (this->capacity() != rhs.capacity()))
        {
            return false;
        }
        /// if we here: this and rhs have same size and capacity

        iterator it = rhs.begin();
        for (int index = 0; index < rhs.size() ; index ++)  // num of iteration = from begin to end
        {
            if (this->at(index) != rhs.at(index))
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const VLVector<T, StaticCapacity> &rhs) const
    {
        if (*this == rhs)
        {
            return false;
        }
        return true;
    }


    /** Iterators */
    /**
     * @brief class of iterator for my VLV
     * @tparam I
     */
    template<class I>
    class IteratorRandomAccess
    {
    private:
        I *_curr; // pointer to _curr element
        size_t i;

    public:
        /** traits */
        typedef I value_type;
        typedef I &reference;
        typedef I *pointer;
        typedef int difference_type;
        typedef std::random_access_iterator_tag random_access_iterator;


        /** constructor */
        IteratorRandomAccess() = default;  // default
        IteratorRandomAccess(I *_c = nullptr, unsigned int i = 0) : _curr(_c), i(i){}

        ~IteratorRandomAccess() = default;  // todo need?

        /// get access
        reference operator*() const
        {
            return *(_curr + i);
        }


        /// go forward - pre
        IteratorRandomAccess &operator++()
        {
            i++;
            return *this;
        }

        /// go forward - post
        IteratorRandomAccess operator++(int)
        {
            IteratorRandomAccess tmp = *this;
            i++;
            return tmp;
        }

        // get value in index
        reference operator[](const int index)
        {
            return (*_curr)[i + index];
        }


        // assign
        IteratorRandomAccess &operator=(const IteratorRandomAccess &rhs)
        {
            if (this == &rhs)  // avoid self-assignment
            {
                return *this;
            }
            _curr = rhs._curr;
            i = rhs.i;
            return *this;  // return ref
        }

        /// go back - pre
        IteratorRandomAccess &operator--()
        {
            i--;
            return *this;
        }

        /// go back - post
        IteratorRandomAccess operator--(int)
        {
            IteratorRandomAccess tmp = *this;
            i--;
            return tmp;
        }

        // add steps
        difference_type operator+(int &steps) const
        {
            return I(_curr, i + steps);
        }

        // decrease steps
        difference_type operator-(int &steps) const
        {
            return I(_curr, i - steps);
        }

        // distance between 2 iterators
        size_t operator-(const IteratorRandomAccess &rhs) const
        {
            return ((this->i) - (rhs.i));
        }

        // add steps +=
        IteratorRandomAccess &operator+=(int &steps)
        {
            i += steps;
            return *this;
        }

        // decrease steps -=
        IteratorRandomAccess &operator-=(int &steps)
        {
            i -= steps;
            return *this;
        }


        // compare equal
        bool operator==(const IteratorRandomAccess &rhs) const
        {
            return this->i == rhs.i;
        }

        // not equal
        bool operator!=(const IteratorRandomAccess &rhs) const
        {
            return this->i != rhs.i;
        }

        // smaller than
        bool operator<(const IteratorRandomAccess &rhs)
        {
            return this->i < rhs.i;
        }

        // bigger than
        bool operator>(const IteratorRandomAccess &rhs)
        {
            return this->i > rhs.i;
        }

        // smaller-equal than
        bool operator<=(const IteratorRandomAccess &rhs)
        {
            return this->i <= rhs.i;
        }

        // bigger-equal than
        bool operator>=(const IteratorRandomAccess &rhs)
        {
            return this->i >= rhs.i;
        }

    };
    typedef IteratorRandomAccess<T> iterator;


    /**
     * @brief class of CONST iterator for my VLV
     * @tparam C
     */

    template<class C>
    class ConstIteratorRandomAccess
    {
    private:
        C *_const_curr; // pointer to _curr element
        size_t i;

    public:
        /** traits */
        typedef const C value_type;
        typedef C &reference;
        typedef C *pointer;
        typedef int difference_type;
        typedef std::random_access_iterator_tag random_access_iterator;


        /** constructor */
        ConstIteratorRandomAccess() = default;  // default
        ConstIteratorRandomAccess(C *_c = nullptr, unsigned int i = 0) : _const_curr(_c), i(i){}

        ~ConstIteratorRandomAccess() = default;  // todo need?

        /// get access
        reference operator*() const
        {
            return *(_const_curr + i);
        }


        /// go foward - pre
        ConstIteratorRandomAccess &operator++()
        {
            i++;
            return *this;
        }

        /// go foward - post
        ConstIteratorRandomAccess operator++(int)
        {
            ConstIteratorRandomAccess tmp = *this;
            i++;
            return tmp;
        }


        // get value in index - CONST
        reference operator[](const int index) const
        {
            return (*_const_curr)[index];
        }

        // assign
        ConstIteratorRandomAccess &operator=(const ConstIteratorRandomAccess &rhs)
        {
            if (this == &rhs)  // avoid self-assignment
            {
                return *this;
            }
            _const_curr = rhs._curr;
            i = rhs.i;
            return *this;  // return ref
        }

        /// go back - pre
        ConstIteratorRandomAccess &operator--()
        {
            i--;
            return *this;
        }

        /// go back - post
        ConstIteratorRandomAccess operator--(int)
        {
            ConstIteratorRandomAccess tmp = *this;
            i--;
            return tmp;
        }

        // add steps
        difference_type operator+(int &steps) const
        {
            return C(_const_curr, i + steps);
        }

        // decrease steps
        difference_type operator-(int &steps) const
        {
            return C(_const_curr, i - steps);
        }

        // distance between 2 iterators
        size_t operator-(const ConstIteratorRandomAccess &rhs) const
        {
            return ((this->i) - (rhs.i));
        }

        // add steps +=
        ConstIteratorRandomAccess &operator+=(int &steps)
        {
            i += steps;
            return *this;
        }

        // decrease steps -=
        ConstIteratorRandomAccess &operator-=(int &steps)
        {
            i -= steps;
            return *this;
        }


        // compare equal
        bool operator==(const ConstIteratorRandomAccess &rhs) const
        {
            return this->i == rhs.i;
        }

        // not equal
        bool operator!=(const ConstIteratorRandomAccess &rhs) const
        {
            return this->i != rhs.i;
        }

        // smaller than
        bool operator<(const ConstIteratorRandomAccess &rhs)
        {
            return this->i < rhs.i;
        }

        // bigger than
        bool operator>(const ConstIteratorRandomAccess &rhs)
        {
            return this->i > rhs.i;
        }

        // smaller-equal than
        bool operator<=(const ConstIteratorRandomAccess &rhs)
        {
            return this->i <= rhs.i;
        }

        // bigger-equal than
        bool operator>=(const ConstIteratorRandomAccess &rhs)
        {
            return this->i >= rhs.i;
        }
    };
    typedef ConstIteratorRandomAccess<const T> const_iterator;


    /** methods for iterators */

    iterator begin()
    {
        if (!this->trueIfHeap)  // so we on stack
        {
            return iterator(_vStack);
        }
        return iterator(_vHeap);  // else - so we on heap
    }

    iterator end()
    {
        return nullptr; // one after the end
    }

    const_iterator begin() const
    {
        if (!this->trueIfHeap)   // so we on stack
        {
            return const_iterator(_vStack);
        }
        return const_iterator(_vHeap);  // else - so we on heap
    }

    const_iterator end() const
    {
        return nullptr;  // one after the end
    }

    const_iterator cbegin() const
    {
        if (!this->trueIfHeap)  // so we on stack
        {
            return const_iterator(_vStack);
        }
        return const_iterator(_vHeap);  // else - so we on heap
    }

    const_iterator cend() const
    {
        return nullptr;
    }

    /** methods */
    int size();

    int capacity();

    bool empty();

    T & at(int index);

    void push_back(const T &element);

    /**
     * @brief insert val to position in VLV
     * @param position
     * @param val
     * @return iterator to this[position]
     */
    iterator insert(iterator position, const T &val) // like gcc sign
    {
        int index = this->size() - 1; // "-1" cause start in 0
        const int numOfElements = this->size();
        iterator helpIterator = ( &( this->at(size()) ) );


        // case 1 - no dynamic at all (stack to stack)
        if (!this->trueIfHeap && (numOfElements < (const int)StaticCapacity))
            //on stack and there space

        {  // push elements with helpIterator to the right
            for (; helpIterator >= position ; helpIterator --)
            {  // we want to push each element to the right
                this->at(index + 1) = (*helpIterator);  // put val in next cell in HEAP
                index -- ; //index -1 (move left) for next iteration
            }
            // after we finish moving - put val in position
            this->at(index) = val;
            //this->_vStack[helpIterator] = val;
            this->sizeOfVLVector++; // increase size by 1
        }

            // case 2 - stack to heap
        else if (!this->trueIfHeap && (numOfElements == StaticCapacity))//on stack- there NO space
        {
            _moveToHeap(); // create dynamic, copy item, update capacity
            // push elements with helpIterator
            for (; helpIterator >= position ; helpIterator --)
            {  // we want to push each element to the right
                this->at(index + 1) = (*helpIterator);  // put val in next cell in HEAP
                index -- ; //index -1 (move left) for next iteration
            }
            // after we finish moving - put val in position
            this->at(index) = val;
            this->sizeOfVLVector++; // increase size by 1
        }

            // case 3 - heap to heap
        else if (this->trueIfHeap && (numOfElements < this->capacity())) //on heap and there space
        {  // push elements with helpIterator
            for (; helpIterator >= position ; helpIterator --) // go back
            {  // we want to push each element to the right
                this->at(index + 1) = (*helpIterator);  // put the val in next cell
                index -- ; //index -1 (move left) for next iteration
            }
            // after we finish moving - put val in position
            this->at(index) = val;
            this->sizeOfVLVector++; // increase size by 1
        }

            // case 4 - heap to increase heap
        else if (this->trueIfHeap && (numOfElements == this->capacity()))//on heap - there NO space
        {
            _increaseHeap(); // create dynamic, copy item, update capacity
            // push elements with helpIterator
            for (; helpIterator >= position ; helpIterator --)
            {  // we want to push each element to the right
                this->at(index + 1) = (*helpIterator);   // put val in next cell in HEAP
                index -- ; //index -1 (move left) for next iteration
            }
            // after we finish moving - put val in position
            this->at(index) = val;
            this->sizeOfVLVector++; // increase size by 1
        }
        return helpIterator;  // return iterator to this[position]
    }

    /**
     * @brief insert interval to VLV in position
     * @tparam InputIterator
     * @param position
     * @param first start of interval
     * @param last end of interval
     * @return iterator
     */
    template<class InputIterator>
    iterator insert (iterator position, InputIterator first, InputIterator last)
    {
        const iterator helpIterator = position;
        for (InputIterator it = first ; it != last ; it ++)
        {
            insert(position, (*it));  // update size_field in the insert (1)
            position ++;
        }
        return helpIterator;
    }

    /**
     * @brief - srase element in position of VLV
     * @param position - pointer to val in VLV we want to erase
     * @return pointer to next element of we erase in VLV
     */
    iterator erase(iterator position)
    {
        // check valid position
        if (position < this->begin() || position > this->end())
        {
            throw Error_INDEX();  // throw error like you want
        }

        size_t indexToErase = (position - begin()); // find the index to erase

        ///  index is the index of array that position points to

        iterator it = position;  // iterator from this[0]
        const int numOfElements = this->size();
        iterator helpIterator = ( &( this->at(size()) ) );
        // iterator point to the size of VLV

        this->at(indexToErase).~T(); // call destructor of T
        /// now move the elements one step left

        // case 1 - no dynamic at all (stack to stack)
        if (!this->trueIfHeap && (numOfElements < (const int)StaticCapacity))  //on stack.
        {
            // we want to push each element to the left
            for (int indexRun = (position - begin()); indexRun < this->size(); indexRun++)
                // go forward until helpIterator (size of VLV)
            {
                this->at(indexToErase) = (this->at(indexRun + 1));  // move the val of next cell
                // one step left

                indexToErase ++ ;  //index + 1 (move right) for next iteration
                indexRun++;
            }
            this->sizeOfVLVector -- ; // decrease size by 1
        }


            // case 2 - heap to heap
        else if (this->trueIfHeap && (numOfElements - 1 > (int)StaticCapacity))//after erase we on
        // heap
        {   // we want to push each element to the left

            for (int indexRun = (position - begin()); indexRun <= this->size(); indexRun++)
                // go forward until helpIterator (size of VLV)
            {
                if ((this->at(indexRun + 1)))                       // if not nullptr
                {
                    this->at(indexToErase) = (this->at(indexRun + 1));  // move the val of next cell
                    // one step left

                }
                else if (!(this->at(indexRun + 1)))                // if NULL
                {
                    this->at(indexToErase).~T();
                }
                indexToErase ++ ;  //index + 1 (move right) for next iteration
                indexRun++;
            }
            this->sizeOfVLVector -- ; // decrease size by 1
        }

            // case 3 - heap to stack
        else if (this->trueIfHeap && (numOfElements == StaticCapacity)) //after erase num < cap
        {

            // we want to push each element to the left
            for (int indexRun = (position - begin()); indexRun <= this->size(); indexRun++)
                // go forward until helpIterator (size of VLV)
            {
                if ((this->at(indexRun + 1)))                       // if not nullptr
                {
                    this->at(indexToErase) = (this->at(indexRun + 1));  // move the val of next cell
                    // one step left

                }
                else if (!(this->at(indexRun + 1)))                // if NULL
                {
                    this->at(indexToErase).~T();
                }
                indexToErase ++ ;  //index + 1 (move right) for next iteration
                indexRun++;
            }

            /// now we move to stack
            _moveToStack(); // copy item to stack, delete heap ,update capacity
            this->sizeOfVLVector -- ; // decrease size by 1
        }
        return position;  // return iterator to this[position]
    }

    /**
     * @brief erase interval from VLV
     * @param first start of interval
     * @param last end of interval
     * @return iterator
     */
    iterator erase(iterator first, iterator last)
    {
        //const int numOfElements = this->size();
        //int numOfErase = last - first;  // how many erase we need (len of interval to erase)
        //for (int i = 0 ; i < numOfErase ; i++)  // numOfErase iterations
        iterator helpIterator = last;
        for (helpIterator ; helpIterator >= first ; helpIterator --)  // go back
        {
            erase(helpIterator);
        }
        return first;
    }


    /**
     * @brief delete last element in VLV
     */
    void pop_back()
    {
        // case 1 : we on stack
        if (!this->trueIfHeap && (this->size() < this->capacity()))
        {
            (this->_vStack[sizeOfVLVector]).~T();
            sizeOfVLVector --; // decrease size by 1
        }

            // case 2 : we on heap and still on heap after pop
        else if (this->trueIfHeap && (this->size() > (int)StaticCapacity))
        {
            (this->_vHeap[sizeOfVLVector]).~T();
            sizeOfVLVector --;  // decrease size by 1
        }
            // case 3 : we on heap and go to stack after pop
        else if (this->trueIfHeap && (this->size() == (int)StaticCapacity))
        {
            _moveToStack();
            (this->_vStack[sizeOfVLVector]).~T();
            sizeOfVLVector --;  // decrease size by 1
        }
    }

    /**
     * @brief delete all elements from VLV
     */
    void clear()
    {
        // case 1 : we on stack - so clean it
        if (!this->trueIfHeap && (this->size() < this->capacity()))
        {
            while (this->size() != 0)
            {
                pop_back();
            }
        }

        // case 2 : we on heap - delete heap
        delete [] this->_vHeap;
        this->capacityOfVLVector = StaticCapacity;
        this->trueIfHeap = false; // we change to stack, so change the switch
    }

    /**
     * @brief return pointer to the current data struct
     * @return return pointer to the current data struct
     */
    T * data()
    {
        if (!this->trueIfHeap)  // so we on stack
        {
            return _vStack;
        }
        return _vHeap;  // else - so we on heap
    }

};


/** operators implement */
template<class T, unsigned int StaticCapacity>
T &VLVector<T, StaticCapacity>::operator[](int i) noexcept
{

    // if we here -> i in range
    if (!this->trueIfHeap) // we on stack
    {
        return _vStack[i];
    }                      // we on heap
    return _vHeap[i];
}

template<class T, unsigned int StaticCapacity>
const T &VLVector<T, StaticCapacity>::operator[](int i) const noexcept
{
    if (i < 0 || i > this->capacity()) // out of range
    {
        return nullptr;
    }
    // if we here -> i in range
    if (!this->trueIfHeap) // we on stack
    {
        return _vStack[i];
    }                      // we on heap
    return _vHeap[i];
}

template<class T, unsigned int StaticCapacity>
VLVector<T, StaticCapacity>  & VLVector<T, StaticCapacity>::operator =
        (VLVector<T, StaticCapacity> & rhs)
{
    if (this != rhs)  // avoid self-assignment
    {
        this->clear();                                  // clear old vector
        long indexToCopy = 0; // index that we want to copy from

        ///  NOW WE HAVE STACK VLV (cause clear())
        // case 1 : to stack
        if ((!rhs.trueIfHeap)) // we need to create stack VLV
        {
            for (indexToCopy ; indexToCopy < this->size() ; indexToCopy ++)
            {
                this->at(indexToCopy) = rhs.at(indexToCopy);
            }
        }
            // case 2 : to heap
        else if ((rhs.trueIfHeap)) // we need to create heap VLV
        {
            this->_vHeap = new T[rhs.cpacity];        // alloc new memory to new dynamic VLV
            for (indexToCopy ; indexToCopy < this->size() ; indexToCopy ++)
            {
                this->at(indexToCopy) = rhs.at(indexToCopy);
            }
        }
        // update primitive fields of VLV
        this->sizeOfVLVector = rhs.sizeOfVLVector;
        this->capacityOfVLVector = rhs.capacityOfVLVector;
        this->trueIfHeap = rhs.trueIfHeap;
    }

    return * this;  // return ref
}


/** methods implement */

/**
 * @brief push new element to the end of VLVector
 * @tparam T
 * @tparam StaticCapacity
 * @param element
 */
template<class T, unsigned int StaticCapacity>
void VLVector<T, StaticCapacity>::push_back(const T & element)
{
    // case 0  - empty VLV
    if (this->empty())
    {
        this->_vStack[0] = element;
        sizeOfVLVector ++; // increase size by 1
    }
    // case 1 : we on stack and have space to element
    else if (!this->trueIfHeap && (this->size() < this->capacity()))
    {
        this->_vStack[sizeOfVLVector] = element;
        sizeOfVLVector ++; // increase size by 1
    }
        // case 2 : we on stack and have NO space to element - move to heap
    else if (!this->trueIfHeap && (this->size() == this->capacity()))
    {
        this->trueIfHeap = true;
        _moveToHeap();
        this->_vHeap[sizeOfVLVector] = element;
        sizeOfVLVector ++;  // increase size by 1
    }
        // case 3 : we on heap and have space to element
    else if (this->trueIfHeap && (this->size() < this->capacity()))
    {
        this->_vHeap[sizeOfVLVector] = element;
        sizeOfVLVector ++;  // increase size by 1
    }
        // case 4 : we on heap and have NO space to element
    else if (this->trueIfHeap && (this->size() == this->capacity()))
    {
        _increaseHeap();
        this->_vHeap[sizeOfVLVector] = element;
        sizeOfVLVector ++;  // increase size by 1
    }
}

/**
 * @brief get size of VLV
 * @tparam T
 * @tparam StaticCapacity
 * @return
 */
template<class T, unsigned int StaticCapacity>
int VLVector<T, StaticCapacity>::size()
{
    return this->sizeOfVLVector;
}

/**
 * @brief get capacity of VLV
 * @tparam T
 * @tparam StaticCapacity
 * @return
 */
template<class T, unsigned int StaticCapacity>
int VLVector<T, StaticCapacity>::capacity()
{
    return this->capacityOfVLVector;
}

/**
 * @brief check if VLV is empty
 * @tparam T
 * @tparam StaticCapacity
 * @return
 */
template<class T, unsigned int StaticCapacity>
bool VLVector<T, StaticCapacity>::empty()
{
    if (this->size() != 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief return val of the input index
 * @tparam T
 * @tparam StaticCapacity
 * @param index
 * @return return val of the input index
 */
template<class T, unsigned int StaticCapacity>
T & VLVector<T, StaticCapacity>::at(int index)
{
    if ((this->empty()) || (index < 0) || (index > this->capacity()))  // empty or invalid index
    {
        throw Error_INDEX();  // throw error like you want
    }
    if (!this->trueIfHeap)  // so we are on stack
    {
        return _vStack[index];
    }
    else                           // so we are on heap
    {
        return _vHeap[index];
    }
}



#endif //EXAM_CPP_VLVECTOR_HPP
