import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    fp_in = open("res/dollar.txt")
    fp_out = open("out/res.txt")

    lines, origin_data, pred_data = [], [], []
    lines = fp_in.readlines()
    # load origin  data
    for line in lines:
        origin_data.append(float(line[:-1]))
    print(origin_data)

    # load prediction data
    lines.clear()
    lines = fp_out.readlines()
    for line in lines:
        pred_data.append(float(line[:-1]))
    print(pred_data)

    # plot
    plt.scatter(np.arange(1, len(origin_data) + 1), origin_data, marker="o", c="r")
    plt.scatter(np.arange(1, len(pred_data) + 1), pred_data, marker="x",c="b")
    plt.legend(['origin','pred'])
    plt.savefig('out/res.jpg');
    plt.show()
