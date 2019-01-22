import numpy as np
import matplotlib.pyplot as plt

def generate_report(time_start, time_stamps):
    time_stamps_minus_offset = np.subtract(time_stamps, time_start)
    time_stamps_aux = np.insert(time_stamps_minus_offset[0:-1], 0, 0)
    transactions_time = np.subtract(time_stamps_minus_offset, time_stamps_aux)
    transactions_time = np.multiply(transactions_time, 1000)

    test_parameters = {
        'trans_number': transactions_time.size,
        'std': np.std(transactions_time),
        'max': np.max(transactions_time),
        'arg_max': np.argmax(transactions_time),
        'min': np.min(transactions_time),
        'arg_min': np.argmin(transactions_time),
        'mean': np.mean(transactions_time),
        'median': np.median(transactions_time)}

    for key, value in test_parameters.items():
        test_parameters[key] = np.around(value, decimals=3)

    fig, ax = plt.subplots(1)
    textstr = "Transaction num: {trans_number} \nStd deviation: {std} [ms] \n" \
              "Max: {max} [ms]  idx: {arg_max} \nMin: {min} [ms]  idx: {arg_min}\nMean: {mean} [ms]" \
              " \nMedian: {median} [ms]".format(**test_parameters)


    ax.hist(transactions_time, 100)
    box_properties = dict(boxstyle='round', facecolor='grey', alpha=0.3)
    ax.text(0.58, 0.95, textstr, transform=ax.transAxes, fontsize=10,
            horizontalalignment='left', verticalalignment='top', bbox=box_properties)

    ax.set_title('Test results')
    ax.set_ylabel('Quantity')
    ax.set_xlabel('Period [ms]')

    plt.show()

def check_corretness_mod256(data, sample_num):
    data_pattern_sum = 0
    for i in range(sample_num):
        data_pattern_sum += i % 256

    data_result = np.equal(data, data_pattern_sum)
    data_correctness = all(data_result)
    return data_correctness

