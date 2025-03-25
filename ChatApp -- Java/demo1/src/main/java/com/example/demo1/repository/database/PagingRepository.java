package com.example.demo1.repository.database;

import com.example.demo1.domain.Entity;
import com.example.demo1.domain.Page;
import com.example.demo1.domain.Pageable;
import com.example.demo1.repository.Repository;

public interface PagingRepository<ID, E extends Entity<ID>> extends Repository<ID, E> {
    Page<E> findAllOnPage(Pageable pageable);
}
