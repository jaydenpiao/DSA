/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){

//index of start
n1=0;
//index of end
n2=-1;

}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL(){
    //save front to return 
    T front = data[n1];
    //increment front
    n1++;
    if (isEmpty()) {
        //empty case
        n1=0;
        n2=-1;
        data.resize(0);
    } else if (n2-n1+1 <= n1) {
        //case where resize is needed
        //size <= start index

        std::vector<T> tempvec;
        
        //copy data to the temp vector
        for (int i = n1; i <= n2; i++) {
            tempvec.push_back(data[i]);
        }
        
        //resize the og data vector
        data.resize(tempvec.size());

        //copy from temp to og
        for (unsigned long i = 0; i<tempvec.size(); i++) {
            data[i] = tempvec[i];
        }

        //reset n1 and n2
        n1=0;
        n2=tempvec.size() -1;
    } 
    return front;
}

/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T back = data[n2+n1];
    data.pop_back();
    n2--;
    
    if (isEmpty()) {
        //empty case
        n1=0;
        n2=-1;
        data.resize(0);
    } else if (n2-n1+1 <= n1) {
        //case where resize is needed
        //size <= start index

        std::vector<T> tempvec;
        
        //copy data to the temp vector
        for (int i = n1; i <= n2; i++) {
            tempvec.push_back(data[i]);
        }
        
        //resize the og data vector
        data.resize(tempvec.size());

        //copy from temp to og
        for (unsigned long i = 0; i<tempvec.size(); i++) {
            data[i] = tempvec[i];
        }

        //reset n1 and n2
        n1=0;
        n2=tempvec.size() -1;
    } 
    return back;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n1 > n2;
}
