from database_init import init_db
from gui_app import main as run_gui_app

DB_FILE = 'sklep.db'

def start_app():
    print(f"[INFO] Uruchamianie aplikacji z bazą danych: {DB_FILE}")
    
    try:
        # Próba inicjalizacji bazy danych
        init_db(DB_FILE)
        print("Baza danych została zainicjalizowana (jeśli jeszcze nie istniała).")
    except Exception as e:
        print(f"[WARNING] Inicjalizacja bazy nie powiodła się: {e}")
    
    print("Uruchamianie GUI...")
    run_gui_app()

if __name__ == "__main__":
    start_app()
