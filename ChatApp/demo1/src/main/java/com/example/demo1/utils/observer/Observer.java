package com.example.demo1.utils.observer;

import com.example.demo1.utils.events.Event;

public interface Observer <E extends Event>{
    void update(E e);
}
