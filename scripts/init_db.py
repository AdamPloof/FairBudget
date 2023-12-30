import sqlite3
import argparse
from os import remove

class TableBuilder:
    def __init__(self, db_path) -> None:
        self.con = sqlite3.connect(db_path)
        self.cur = self.con.cursor()

    def build_tables(self) -> None:
        self.enable_foreign_keys()
        self.create_person_tbl()
        self.create_expense_tbl()
        self.create_payments_tbl()

    def check_foreign_key_enabled(self) -> bool:
        q = "PRAGMA foreign_keys"
        res = self.cur.execute(q)
        foreign_key_status = res.fetchone()[0]
        
        return True if foreign_key_status == 1 else False

    def enable_foreign_keys(self) -> None:
        q = "PRAGMA foreign_keys = ON;"
        self.cur.execute(q)
        self.con.commit()

    def create_person_tbl(self) -> None:
        q = """
            CREATE TABLE IF NOT EXISTS person (
                id INTEGER PRIMARY KEY,
                name TEXT NOT NULL,
                income INTEGER NOT NULL
            )
        """
        self.cur.execute(q)

    def create_expense_tbl(self) -> None:
        q = """
            CREATE TABLE IF NOT EXISTS expense (
                id INTEGER PRIMARY KEY,
                description TEXT NOT NULL,
                amount INTEGER NOT NULL
            )
        """
        self.cur.execute(q)

    def create_payments_tbl(self) -> None:
        q = """
            CREATE TABLE IF NOT EXISTS payment (
                id INTEGER PRIMARY KEY,
                paid_by INTEGER NOT NULL,
                expense INTEGER NOT NULL,
                amount INTEGER NOT NULL,
                FOREIGN KEY (paid_by)
                    REFERENCES person (id)
                    ON DELETE CASCADE,
                FOREIGN KEY (expense)
                    REFERENCES expense(id)
                    ON DELETE CASCADE
            )
        """
        self.cur.execute(q)

    def close(self) -> None:
        self.con.close()


class FixturesManager:
    def __init__(self, db_path) -> None:
        self.con = sqlite3.connect(db_path)
        self.cur = self.con.cursor()
        pass

    def insert_fixtures(self) -> None:
        self.insert_people()
        self.insert_expenses()
        self.insert_payments()
        print('Fixtures inserted.')

    def insert_people(self) -> None:
        q = """
            INSERT INTO person (name, income)
            VALUES
                ('Ted', 35000),
                ('Sherry', 65000)
        """
        self.cur.execute(q)
        self.con.commit()

    def insert_expenses(self) -> None:
        q = """
            INSERT INTO expense (description, amount)
            VALUES
                ('Rent', 1200),
                ('TV', 65),
                ('Car', 290),
                ('Electricity', 80),
                ('Gas', 150)
        """
        self.cur.execute(q)
        self.con.commit()

    def insert_payments(self) -> None:
        personIds = self.get_person_ids()
        expenses = self.get_expense_ids()

        payments = []
        cnt = 0
        while cnt < 4:
            personIdx = 0 if (cnt % 2) > 0 else 1
            payments.append((personIds[personIdx], expenses[cnt][0], expenses[cnt][1]))
            cnt += 1

        q = """
            INSERT INTO payment (paid_by, expense, amount)  
            VALUES (?, ?, ?)
        """

        self.cur.executemany(q, payments)
        self.con.commit()

    def get_person_ids(self) -> list[int]:
        q = "SELECT id FROM person"
        res = self.cur.execute(q)
        persons = res.fetchall()

        return [p[0] for p in persons]
    
    def get_expense_ids(self) -> list[tuple]:
        q = "SELECT id, amount FROM expense"
        res = self.cur.execute(q)
        expenses = res.fetchall()

        return expenses

    def clear_fixtures(self):
        self.clear_table('person')
        self.clear_table('expense')
        self.clear_table('payment')

    def clear_table(self, tbl: str) -> None:
        q = f"DELETE FROM {tbl} WHERE id > 0"
        self.cur.execute(q)

    def close(self) -> None:
        self.con.close()


def create_tables(db_path: str) -> None:
    builder = TableBuilder(db_path)
    builder.enable_foreign_keys()

    if builder.check_foreign_key_enabled():
        print('Database is setup. Foreign keys are enabled')
    else:
        print('Database is setup. Foreign keys are not enabled')

    builder.build_tables()
    builder.close()

def insert_fixtures(db_path: str) -> None:
    fixtures = FixturesManager(db_path)
    fixtures.insert_fixtures()
    fixtures.close()


def remove_existing(db_path: str) -> None:
    print(f"Removing existing db: {db_path}")
    remove(db_path)

def main():
    DB_PATH = '../../data.db'

    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-c', '--clean',
        help='Remove existing database if one already exists and start fresh.',
        action='store_true'
    )
    args = parser.parse_args()

    if args.clean:
        remove_existing(DB_PATH)

    create_tables(DB_PATH)
    insert_fixtures(DB_PATH)


if __name__ == '__main__':
    main()
