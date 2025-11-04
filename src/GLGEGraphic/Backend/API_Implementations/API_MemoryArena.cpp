/**
 * @file GLGEGraphicAPI_MemoryArena.cpp
 * @author DM8AT
 * @brief implement the default functionality of a memory arena
 * @version 0.1
 * @date 2025-06-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the GPU memory arena
#include "API_MemoryArena.h"

//add memcpy
#include <cstring>

GLGE::Graphic::Backend::API::MemoryArena::~MemoryArena()
{
    //clear all vectors
    m_free.clear();
    //say that no data is stored
    m_size = 0;
}

GLGE::Graphic::Backend::API::MemoryArena::GraphicPointer GLGE::Graphic::Backend::API::MemoryArena::allocate(uint64_t size) noexcept
{
    //iterate over all free regions
    for (uint64_t i = 0; i < m_free.size(); ++i)
    {
        //check if the free area is sufficient
        if (m_free[i].size >= size)
        {
            //store the index
            uint64_t idx = m_free[i].startIdx;

            //push the found region forward by the size and remove the now allocated region
            m_free[i].startIdx += size;
            m_free[i].size -= size;

            //check if the size is 0 to save RAM by deleting the region
            if (m_free[i].size == 0)
            {
                //erase the region
                m_free.erase(m_free.begin()+i);
            }

            //return the found pointer
            return {idx, size};
        }
    }

    //before resizing, check if resizing is allowed
    if (!m_allowResize)
    {
        //if not, stop
        return {0,0};
    }

    //store if the last element should be extended
    bool m_extend = m_free.size() > 0;
    //check if extend is currently possible
    if (m_extend)
    {
        //check if it is also the correct one
        m_extend = ((m_free[m_free.size()-1].startIdx + m_free[m_free.size()-1].size) == m_size);
    }
    //not enough free space is found. Append or grow the end of the array to fit the data
    if (m_extend)
    {
        //the last element reaches to the end of the array. Just increase the size. 

        //calculate the new size
        uint64_t nSize = m_size + (size - m_free[m_free.size()-1].size);

        //resize the memory arena
        resize(nSize);

        //store the index of the last element
        uint64_t idx = m_free[m_free.size()-1].startIdx;
        //erase the last element. It is now full. 
        m_free.erase(m_free.end());

        //return the last element
        return {idx, size};
    }
    else
    {
        //the last element is allready filled. Add a new element to the end. 

        //store the index as the current size
        uint64_t idx = m_size;
        //change the size by the missing size
        resize(m_size + size);
        //return a new pointer to the end of the array
        return {idx, size};
    }
}

bool GLGE::Graphic::Backend::API::MemoryArena::release(GraphicPointer& ptr) noexcept
{
    //store the index the free section will be inserted at
    uint64_t insert = 0;
    //safety check if the requested area is not free
    //check all free segments for overlap with the pointer
    for (uint64_t i = 0; i < m_free.size(); ++i)
    {
        //get a reference to the current pointer
        const GraphicPointer& pointer = m_free[i];
        //check for overlap
        if (((pointer.startIdx < ptr.startIdx) && ((pointer.startIdx + pointer.size) <= ptr.startIdx)) || 
            ((pointer.startIdx > ptr.startIdx) && (pointer.startIdx >= (ptr.startIdx + ptr.size))))
        {
            //check if a next element exists
            if (i != m_free.size()-1)
            {
                //check if the element is in between this and the next element
                //it is allready ensured to not collide, so only check the pointer indices
                //collision with the next element is not checked, so make sure to add the length
                //of the region to the pointer to prevent overlap
                if ((pointer.startIdx < ptr.startIdx) && ((ptr.startIdx + ptr.size) <= m_free[i+1].startIdx))
                {
                    //store this as the insertion index
                    insert = i;
                }
            }
            continue;
        }

        //return the intersection
        return false;
    }

    //add the free section

    //first, check if a free section aligns with another free section
    //check if a section at this one exists (if free sections exist)
    if (m_free.size() != 0)
    {
        //check the section before this one
        //first, fetch the section
        GraphicPointer& pointer = m_free[insert];
        //now, check if the end section perfectly aligns with the start section
        if ((pointer.startIdx + pointer.size) == ptr.startIdx)
        {
            //just increase the size of the region before this one
            pointer.size += ptr.size;

            //check if the section should merge with the next one
            //first, check if a next one exists
            if (insert != m_free.size()-1)
            {
                //fetch the next section
                GraphicPointer& next = m_free[insert+1];
                //check if the end aligns with the next section
                if ((pointer.startIdx + pointer.size) == next.startIdx)
                {
                    //merge the sections by growing the first one by the size of the second one
                    pointer.size += next.size;
                    //erase the later section
                    m_free.erase(m_free.begin() + insert + 1);
                }
            }

            //successfully freed it
            return true;
        }

        //now, check if a section is after this one exists
        if (insert != m_free.size()-1)
        {
            //first, fetch the section
            GraphicPointer& next = m_free[insert+1];
            //check if the end of the to free pointer aligns with the start of the next one
            if ((ptr.startIdx + ptr.size) == next.startIdx)
            {
                //decrease the start index and increase the size by the size of the free'd pointer
                next.startIdx -= ptr.size;
                next.size += ptr.size;

                //check if a section should merge with the one before this
                //first, check if there is a section before this one
                if (insert != 0)
                {
                    //check if the sections aligns
                    if ((pointer.startIdx + pointer.size) == next.startIdx)
                    {
                        //grow the previous section to include this one
                        pointer.size += next.size;
                        //erase this section
                        m_free.erase(m_free.begin() + insert + 1);
                    }
                }

                //free'd the pointer successfully
                return true;
            }
        }
    }

    //no alignment found. Insert the freed section. 
    m_free.insert(m_free.begin() + insert, ptr);

    //return the successful freeing
    return true;
}

void GLGE::Graphic::Backend::API::MemoryArena::update(const GraphicPointer& ptr, void* data) noexcept
{
    //just write the data to the GPU buffer
    m_buff->write(data, ptr.size, ptr.startIdx);
}

void GLGE::Graphic::Backend::API::MemoryArena::resize(uint64_t size) noexcept
{
    //just resize the underlying buffer
    m_buff->resize(size);
}