import serial
import xlwt
from datetime import datetime
import os

class SerialToExcel:

    def __init__(self, port, speed):
        self.port = port
        self.speed = speed

        self.wb = xlwt.Workbook()
        self.ws = self.wb.add_sheet("Data from Serial", cell_overwrite_ok=True)
        self.ws.write(0, 0, "Data from Serial")
        self.columns = ["Date Time"]
        self.number = 100

    def setColumns(self, col):
        self.columns.extend(col)

    def setRecordsNumber(self, number):
        self.number = number

    def readPort(self):
        try:
            ser = serial.Serial(self.port, self.speed, timeout=1)
        except serial.SerialException as e:
            print(f"Error opening serial port: {e}")
            return

        c = 0
        for col in self.columns:
            self.ws.write(1, c, col)
            c += 1
        self.fila = 2

        i = 0
        while i < self.number:
            line = ser.readline().decode('utf-8').strip()
            if len(line) > 0:
                now = datetime.now()
                date_time = now.strftime("%m/%d/%Y, %H:%M:%S")
                print(date_time, line)
                if "," in line:
                    c = 1
                    self.ws.write(self.fila, 0, date_time)
                    columnas = line.split(",")
                    for col in columnas:
                        try:
                            # Intentar convertir la cadena a un número (float)
                            num = float(col)
                            self.ws.write(self.fila, c, num)
                        except ValueError:
                            # Si no es un número, escribir como texto
                            self.ws.write(self.fila, c, col)
                        c += 1

                    i += 1
                    self.fila += 1

    def writeFile(self, archivo):
        print(f"Saving file to {archivo}")
        self.wb.save(archivo)

# Uso del código
if __name__ == "__main__":
    output_file = "C:/Users/Emilio/Downloads/Tesis/serialToExcel-master/archivo1.xls"
    if os.path.exists(output_file):
        os.remove(output_file)  # Elimina el archivo anterior si existe

    serialToExcel = SerialToExcel("COM3", 9600)
    serialToExcel.setColumns(["pH", "Temperature"])
    serialToExcel.setRecordsNumber(5)
    serialToExcel.readPort()
    serialToExcel.writeFile(output_file)
