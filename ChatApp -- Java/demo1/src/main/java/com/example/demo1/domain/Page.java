package com.example.demo1.domain;

import java.util.List;

public class Page<T> {
    private List<T> content;
    private int totalElements;

    public Page(List<T> content, int totalElements) {
        this.content = content;
        this.totalElements = totalElements;
    }

    public List<T> getContent() {
        return content;
    }

    public int getTotalElements() {
        return totalElements;
    }
}