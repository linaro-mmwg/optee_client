/*
 * Copyright (c) 2014, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef HANDLE_H
#define HANDLE_H

#include <stdint.h>
#include <pthread.h>

struct handle_db {
	void **ptrs;
	size_t max_ptrs;
	pthread_mutex_t *mu;
};

#define HANDLE_DB_INITIALIZER { NULL, 0, NULL }
#define HANDLE_DB_INITIALIZER_WITH_MUTEX(mu) { NULL, 0, (mu) }

/*
 * Assigns a mutex for the database. If mu != NULL the mutex will be
 * acquired before each access to the database and released when
 * the operation is done.
 */
void handle_db_set_mutex(struct handle_db *db, pthread_mutex_t *mu);

/*
 * Frees all internal data structures of the database, but does not free
 * the db pointer. The database is safe to reuse after it's destroyed, it
 * just be empty again. The assigned mutex is also preserved.
 */
void handle_db_destroy(struct handle_db *db);

/*
 * Allocates a new handle and assigns the supplied pointer to it,
 * ptr must not be NULL.
 * The function returns
 * >= 0 on success and
 * -1 on failure
 */
int handle_get(struct handle_db *db, void *ptr);

/*
 * Deallocates a handle. Returns the assiciated pointer of the handle
 * the the handle was valid or NULL if it's invalid.
 */
void *handle_put(struct handle_db *db, int handle);

/*
 * Returns the assiciated pointer of the handle if the handle is a valid
 * handle.
 * Returns NULL on failure.
 */
void *handle_lookup(struct handle_db *db, int handle);

void handle_foreach_put(struct handle_db *db,
			void (*cb)(int handle, void *ptr, void *arg),
			void *arg);

#endif /*HANDLE_H*/
