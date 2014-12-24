#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
	int max_rows;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    Database_close(conn);
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database. 1", conn);
    rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database. 2", conn);
    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    for (int i = 0; i < conn->db->max_rows; i++) 
	{
		rc = fread(&(conn->db->rows[i].id), sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to load database. 3", conn);
		rc = fread(&(conn->db->rows[i].set), sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to load database. 4", conn);
        conn->db->rows[i].name = malloc(sizeof(char) * conn->db->max_data);  
		rc = fread(conn->db->rows[i].name, sizeof(char), conn->db->max_data, conn->file);
		if(rc != conn->db->max_data) die("Failed to load database. 5", conn);
        conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);  
		rc = fread(conn->db->rows[i].email, sizeof(char), conn->db->max_data, conn->file);
		if(rc != conn->db->max_data) die("Failed to load database. 6", conn);
	}  
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(&(conn->db->max_data), sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database. 1", conn);
	rc = fwrite(&(conn->db->max_rows), sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database. 2", conn);
    for (int i = 0; i < conn->db->max_rows; i++) 
	{
		rc = fwrite(&(conn->db->rows[i].id), sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to write database. 3", conn);
		rc = fwrite(&(conn->db->rows[i].set), sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to write database. 4", conn);
        rc = fwrite(conn->db->rows[i].name, sizeof(char), conn->db->max_data, conn->file);
		if(rc != conn->db->max_data) die("Failed to write database. 5", conn);
        conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);
		rc = fwrite(conn->db->rows[i].email, sizeof(char), conn->db->max_data, conn->file);
		if(rc != conn->db->max_data) die("Failed to write database. 6", conn);
	}

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) 
		{
			for (int i = 0; i < conn->db->max_rows; i++)
			{
				free(conn->db->rows[i].name);
				free(conn->db->rows[i].email);
			}
			free(conn->db->rows);
			free(conn->db);
		}
        free(conn);
    }
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
	conn->db->max_data = max_data;
    conn->db->max_rows = max_rows;
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    for(int i = 0; i < max_rows; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0, .name=malloc(sizeof(char)*max_data), .email=malloc(sizeof(char)*max_data)};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->name, name, conn->db->max_data);
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, conn->db->max_data);
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 5) die("USAGE: ex17 <dbfile> <max_data> <max_rows> <action> [action params]", 0);
    int max_data = MAX_DATA;
	int max_rows = MAX_ROWS;
    char *filename = argv[1];
    max_data = atoi(argv[2]);
	max_rows = atoi(argv[3]);
    char action = argv[4][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 5) id = atoi(argv[5]);
    if(id >= max_rows) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn, max_data, max_rows);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 6) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 8) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[6], argv[7]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 6) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}

