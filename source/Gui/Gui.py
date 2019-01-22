import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt

COLORS_LIST = ('darkblue', 'red', 'green', 'gold')
CHANNEL_LIST =('CH1', 'CH2', 'CH3', 'CH4')

class GUI:
    def __init__(self, channel_nr, samples_nr):

        self.fig, self.ax = plt.subplots(2,1,sharex = True)
        self.fig.subplots_adjust(hspace=0)
        self.fig.canvas.mpl_connect('key_press_event', self.__set_key)
        self.fig.canvas.set_window_title('Oscilloscope')
        self.fig.set_size_inches(12, 6)

        self.__channel_nr = channel_nr
        self.__sample_nr = samples_nr
        self.__line = list()
        self.__key = None
        self.__pause = False

        self.__state = self.ax[0].text(0.95, 0.9, 'run', horizontalalignment='center',
                                verticalalignment='center', transform=self.ax[0].transAxes,
                                color='green', fontsize=14, fontweight='bold')

        self.fig.text(0.5, 0.03, 'Time [us]', ha='center', fontsize=14)
        self.fig.text(0.07, 0.5, 'Amplitude [V]', va='center', rotation='vertical', fontsize=14)
        self.fig.text(0.01, 0.01, 'Keyboard commands: Space-stop/start  Esc-close', ha='left', fontsize= 10, style='italic')

        for channel in range(self.__channel_nr):
            canal, = self.ax[channel].plot(list(0 for i in range(samples_nr)),
                                           #marker = '.', linewidth=0.5,
                                           color = COLORS_LIST[channel], label =CHANNEL_LIST[channel])
            self.ax[channel].axis([0, samples_nr, 0, 3.9])
            self.ax[channel].tick_params(which='both', direction = 'in', length=3, width=1, labelsize = 14,
                                    bottom=True, left=True, top=True, right=True,grid_alpha=0.3)
            self.__line.insert(channel, canal)
            if channel > 0: self.ax[channel].spines['top'].set_visible(False)
            if channel < channel_nr - 1: self.ax[channel].spines['bottom'].set_visible(False)

        self.ax[0].legend(self.__line, CHANNEL_LIST[:self.__channel_nr] ,loc=2, ncol=2, mode="expand",
                          borderaxespad=0., fontsize=14,bbox_to_anchor=(0., 1.08, 1., .102))

        self.fig.canvas.draw()
        plt.pause(1)

    def __set_key(self, event):
        self.__key = event.key

    def toggle_state(self):
        self.__pause = not self.__pause
        if self.__pause:
            self.__state.set_text('stop')
            self.__state.set_color('red')
        else:
            self.__state.set_text('run')
            self.__state.set_color('green')

    def set_y(self, channel, data):
        self.__line[channel].set_ydata([float(y/77.0) for y in data[:self.__sample_nr]])

    def plot(self):
        try:
           for channel in range(self.__channel_nr):
               self.ax[channel].draw_artist(self.__line[channel])
           self.fig.canvas.update()
        except:
            pass

    def pause(self):
        plt.pause(1e-2)

    def get_key(self):
        key = self.__key
        if  key:
            self.__key = None
        return key