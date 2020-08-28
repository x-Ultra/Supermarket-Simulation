import csv
from time import sleep

filename = "../simulation_results_all.csv"

def start_selecting():

    # dizionatio doppio
    results = {}


    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile,quotechar='"')

        for row in reader:
            if results.get(row["Tipo Cinfig. Cassa"], None) is None:
                results[row['Tipo Cinfig. Cassa']] = {row[' Giorno']: [row]}
            else:
                if results[row["Tipo Cinfig. Cassa"]].get(row[" Giorno"], None) is None:
                    results[row['Tipo Cinfig. Cassa']][row[' Giorno']] = [row]
                else:
                    results[row['Tipo Cinfig. Cassa']][row[' Giorno']].append(row)


    migliori_file = "migliori.csv"
    # per ogni configurazione seleziona le performance migliori per ogni giorno (7 righe per configurazione)
    migliori = {}
    migliori_data = []

    vincenti_file = "vincenti.csv"
    # per ogni giorno seleziona la configurazione migliore (7 righe in totale)
    vincenti = []


    for config_name in results:

        migliori[config_name] = {}

        for day in results[config_name]:

            migliori[config_name][day] = {}

            best_row_day = float('inf')
            day_selected = {}
            for row in results[config_name][day]:
                if(int(row[" Costo Mensile"][1:].split(".")[0]) <= best_row_day):
                    day_selected = row
                    best_row_day = int(row[" Costo Mensile"][1:].split(".")[0])

            migliori[config_name][day] = day_selected
            migliori_data.append(day_selected)


    for day in migliori["Condivisa"]:
        best_row_day = float('inf')
        day_selected = {}
        for config_name in migliori:

            if(int(migliori[config_name][day][" Costo Mensile"][1:].split(".")[0]) <= best_row_day):
                day_selected = migliori[config_name][day]
                best_row_day = int(migliori[config_name][day][" Costo Mensile"][1:].split(".")[0])


        print("vince: ", day_selected[" Giorno"], day_selected["Tipo Cinfig. Cassa"], day_selected[" Costo Mensile"].split(".")[0])
        vincenti.append(day_selected)



    #scrivi su file
    try:
        f = open(vincenti_file, 'w')
        f.write("Tipo Cinfig. Cassa, Num Cassieri, Giorno, IC Tempo Attesa, IC Slowdown, IC SD Tempo Att, IC Abbandoni, IC Arrivi, Lungh. Fila Tollerata, Costo Mensile\n")
        for v in vincenti:
            f.write(v["Tipo Cinfig. Cassa"])
            f.write(", ")
            f.write(v[" Num Cassieri"])
            f.write(", ")
            f.write(v[" Giorno"])
            f.write(", ")
            f.write(v[" IC Tempo Attesa"])
            f.write(", ")
            f.write(v[" IC Slowdown"])
            f.write(", ")
            f.write(v[" IC SD Tempo Att"])
            f.write(", ")
            f.write(v[" IC Abbandoni"])
            f.write(", ")
            f.write(v[" IC Arrivi"])
            f.write(", ")
            f.write(v[" Lungh. Fila Tollerata"])
            f.write(", ")
            f.write(v[" Costo Mensile"])
            f.write("\n")
    except IOError:
        print("I/O error")


    try:
        f = open(migliori_file, 'w')
        f.write("Tipo Cinfig. Cassa, Num Cassieri, Giorno, IC Tempo Attesa, IC Slowdown, IC SD Tempo Att, IC Abbandoni, IC Arrivi, Lungh. Fila Tollerata, Costo Mensile\n")
        for v in migliori_data:
            f.write(v["Tipo Cinfig. Cassa"])
            f.write(", ")
            f.write(v[" Num Cassieri"])
            f.write(", ")
            f.write(v[" Giorno"])
            f.write(", ")
            f.write(v[" IC Tempo Attesa"])
            f.write(", ")
            f.write(v[" IC Slowdown"])
            f.write(", ")
            f.write(v[" IC SD Tempo Att"])
            f.write(", ")
            f.write(v[" IC Abbandoni"])
            f.write(", ")
            f.write(v[" IC Arrivi"])
            f.write(", ")
            f.write(v[" Lungh. Fila Tollerata"])
            f.write(", ")
            f.write(v[" Costo Mensile"])
            f.write("\n")
    except IOError:
        print("I/O error")



    # enjoy




if __name__ == "__main__":
    start_selecting()