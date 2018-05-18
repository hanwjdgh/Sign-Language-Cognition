import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn import metrics
from sklearn.model_selection import train_test_split
import serial
import pyttsx3

engine = pyttsx3.init()

text = pd.read_csv("sample.txt", header=None)
ser = serial.Serial("COM9", 9600)

label = []
data = []

for row_index, row in text.iterrows():
    label.append(row.ix[5])
    row_data = []
    for v in row.ix[:4]:
        row_data.append(v)
    data.append(row_data)

while 1 == 1:
    val = ser.readline()
    if(val):
        d = open("data.txt",'w')
        d.write(val.decode('utf8'))
        d.close()
        o_text = pd.read_csv("data.txt", header=None)
        o_data = []
        for row_index, row in o_text.iterrows():
            row_data = []
            for v in row.ix[:4]:
                row_data.append(v)
            o_data.append(row_data)

    clf = RandomForestClassifier()
    clf.fit(data, label)
    predict = clf.predict(o_data)
    print(predict)
    engine.say(str(predict))
    engine.runAndWait()

"""
clf = RandomForestClassifier()
clf.fit(data_train, label_train)
predict = clf.predict(data_test)

ac_score = metrics.accuracy_score(label_test, predict)
cl_report = metrics.classification_report(label_test, predict)
print("정답률 =", ac_score)
print("리포트 =\n", cl_report)
"""