const path = require('path');

module.exports = {
  context: __dirname,
  entry: './lib/es6/src/index',
  output: {
    filename: '[name].js',
    path: path.join(__dirname, './dist/build'),
    publicPath: '/build/',
  },
  devServer: {
    contentBase: path.resolve(__dirname, 'public'),
    historyApiFallback: true
  },
  resolve: {
    alias: {
      src: path.resolve(__dirname, 'src/'),
      director: 'director/build/director',
    },
  }
};
