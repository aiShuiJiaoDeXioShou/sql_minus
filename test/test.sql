CREATE TABLE students (
    id INT PRIMARY KEY,
    name CHAR(30)
);
CREATE TABLE students (id INT PRIMARY KEY,name CHAR(30));
DELETE FROM students WHERE id = 1;
UPDATE students SET name = 'Bob' WHERE id = 1;
SELECT * FROM students ORDER BY id ASC;
INSERT INTO students VALUES (1, 'Alice');